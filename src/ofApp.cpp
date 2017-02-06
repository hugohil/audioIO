#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(34, 34, 34);
  ofSetFrameRate(60);

  string address = "http://" + ofToString(HOST) + ":" + ofToString(PORT);
  socketIO.setup(address);

  ofAddListener(socketIO.connectionEvent, this, &ofApp::onConnection);

  // ofxDatGuiLog::quiet();

  // ofTrueTypeFont::setGlobalDpi(72);
  verdana.load("gui_assets/fonts/verdana.ttf", 22, true, true);

  soundStream.printDeviceList();
  deviceList = soundStream.getDeviceList();

  sampleRate = 44100;
  bufferSize = 512;

  audioSetupGUI = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  audioSetupGUI->addHeader("DEVICE CONFIG");
  audioSetupGUI->addFooter();
  audioSetupGUI->addFRM();

  ofxDatGuiSlider* RMSThresholdSlider = audioSetupGUI->addSlider("RMS threshold", 0.0, 1.0);
  RMSThresholdSlider->bind(RMSThreshold);

  vector<string> deviceNames;
  for (vector<ofSoundDevice>::iterator device = deviceList.begin(); device != deviceList.end(); ++device) {
    deviceNames.push_back(device->name);
  }
  audioSetupGUI->addDropdown("devices", deviceNames)->onDropdownEvent(this, &ofApp::onDevicesDropdownEvent);
}

void ofApp::onConnection(){
  string key = "register";
  string message = "{\"clientName\": \"audioIO\"}";
  socketIO.emit(key, message);
}

//--------------------------------------------------------------
void ofApp::onDevicesDropdownEvent(ofxDatGuiDropdownEvent e){
  soundStream.stop();
  audioAnalyzer.exit();

  ofSoundDevice device = deviceList[e.child];

  // get device config
  outChannels = device.outputChannels;
  inChannels = device.inputChannels;

  // setup the sound stream
  soundStream.setDevice(device);
  soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, 3);
  // setup ofxAudioAnalyzer with the SAME PARAMETERS
  audioAnalyzer.setup(sampleRate, bufferSize, inChannels);
}


//--------------------------------------------------------------
void ofApp::update(){
  ofSetWindowTitle("AudioIO - " + ofToString(ofGetFrameRate()));

  // get the analysis values for every input channel and send it
  for (int i = 0; i < inChannels; ++i) {
    float rms = audioAnalyzer.getValue(RMS, i);

    if (rms > RMSThreshold) {
      // string param = "{ \"channel\": " + ofToString(i) + ",";
      string param = "{";

      param += "\"rms\":" + ofToString(rms) + ",";
      float power = audioAnalyzer.getValue(POWER, i);
      param += "\"power\":" + ofToString(power) + ",";
      // float pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, i);
      // param += "\"pitchFreq\":" + ofToString(pitchFreq) + ",";
      // float pitchConf = audioAnalyzer.getValue(PITCH_CONFIDENCE, i);
      // param += "\"pitchConf\":" + ofToString(pitchConf) + ",";
      float pitchSalience = audioAnalyzer.getValue(PITCH_SALIENCE, i);
      param += "\"pitchSalience\":" + ofToString(pitchSalience) + ",";
      float inharmonicity = audioAnalyzer.getValue(INHARMONICITY, i);
      param += "\"inharmonicity\":" + ofToString(inharmonicity) + ",";
      // float hfc = audioAnalyzer.getValue(HFC, i);
      // param += "\"hfc\":" + ofToString(hfc) + ",";
      // float specComp = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, i);
      // param += "\"specComp\":" + ofToString(specComp) + ",";
      // float centroid = audioAnalyzer.getValue(CENTROID, i);
      // param += "\"centroid\":" + ofToString(centroid) + ",";
      // float rollOff = audioAnalyzer.getValue(ROLL_OFF, i);
      // param += "\"rollOff\":" + ofToString(rollOff) + ",";
      // float oddToEven = audioAnalyzer.getValue(ODD_TO_EVEN, i);
      // param += "\"oddToEven\":" + ofToString(oddToEven) + ",";
      // float strongPeak = audioAnalyzer.getValue(STRONG_PEAK, i);
      // param += "\"strongPeak\":" + ofToString(strongPeak) + ",";
      // float strongDecay = audioAnalyzer.getValue(STRONG_DECAY, i);
      // param += "\"strongDecay\":" + ofToString(strongDecay) + ",";
      float pitchFreqNorm = audioAnalyzer.getValue(PITCH_FREQ, i);
      param += "\"pitchFreqNorm\":" + ofToString(pitchFreqNorm) + ",";
      float hfcNorm = audioAnalyzer.getValue(HFC, i);
      param += "\"hfcNorm\":" + ofToString(hfcNorm) + ",";
      float specCompNorm = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, i);
      param += "\"specCompNorm\":" + ofToString(specCompNorm) + ",";
      float centroidNorm = audioAnalyzer.getValue(CENTROID, i);
      param += "\"centroidNorm\":" + ofToString(centroidNorm) + ",";
      float rollOffNorm = audioAnalyzer.getValue(ROLL_OFF, i);
      param += "\"rollOffNorm\":" + ofToString(rollOffNorm) + ",";
      float oddToEvenNorm = audioAnalyzer.getValue(ODD_TO_EVEN, i);
      param += "\"oddToEvenNorm\":" + ofToString(oddToEvenNorm) + ",";
      float strongPeakNorm = audioAnalyzer.getValue(STRONG_PEAK, i);
      param += "\"strongPeakNorm\":" + ofToString(strongPeakNorm) + ",";
      float strongDecayNorm = audioAnalyzer.getValue(STRONG_DECAY, i);
      param += "\"strongDecayNorm\":" + ofToString(strongDecayNorm) + ",";
      float dissonance = audioAnalyzer.getValue(DISSONANCE, i);
      param += "\"dissonance\":" + ofToString(dissonance);

      param += "}";

      string eventName = "channel-" + ofToString(i);
      socketIO.emit(eventName, param);

      // Those are vectors. Will see later how to send them through OSC.
      // vector spectrum = audioAnalyzer.getValues(SPECTRUM, 0);
      // vector melBands = audioAnalyzer.getValues(MEL_BANDS, 0);
      // vector mfcc = audioAnalyzer.getValues(MFCC, 0);
      // vector hpcp = audioAnalyzer.getValues(HPCP, 0);
      // vector tristimulus = audioAnalyzer.getValues(TRISTIMULUS, 0);
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &inBuffer){
  float alpha = 1.0;
  float silenceThreshold = 0.2;
  float useTimeThreshold = true;
  float timeThreshold = 100.0; // in ms.
  audioAnalyzer.setOnsetsParameters(0, alpha, silenceThreshold, timeThreshold, useTimeThreshold);
  audioAnalyzer.analyze(inBuffer);
}

//--------------------------------------------------------------
void ofApp::exit(){
  audioAnalyzer.exit();
  soundStream.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
