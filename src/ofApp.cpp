#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(34, 34, 34);
  ofSetFrameRate(60);

  string address = "http://" + ofToString(HOST) + ":" + ofToString(PORT);
  socketIO.setup(address);

  ofAddListener(socketIO.connectionEvent, this, &ofApp::onConnection);

  verdana.load("gui_assets/fonts/verdana.ttf", 22, true, true);

  soundStream.printDeviceList();
  deviceList = soundStream.getDeviceList();

  sampleRate = 44100;
  bufferSize = 512;

  ofxDatGuiLog::quiet();

  audioSetupGUI = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  audioSetupGUI->addHeader("DEVICE CONFIG");
  audioSetupGUI->addFooter();
  audioSetupGUI->addFRM();

  ofxDatGuiSlider* RMSThresholdSlider = audioSetupGUI->addSlider("RMS threshold", 0.0, 1.0);
  RMSThresholdSlider->bind(RMSThreshold);

  ofxDatGuiSlider* smoothingSlider = audioSetupGUI->addSlider("smoothing", 0.0, 1.0);
  smoothingSlider->bind(smoothing);

  ofxDatGuiFolder* onsets = audioSetupGUI->addFolder("onsets", ofColor::white);

  ofxDatGuiSlider* onSetsAlphaSlider = onsets->addSlider("alpha", 0.0, 1.0);
  onSetsAlphaSlider->bind(onSetsAlpha);

  ofxDatGuiSlider* onSetsSilenceThresholdSlider = onsets->addSlider("silenceThreshold", 0.0, 1.0);
  onSetsSilenceThresholdSlider->bind(onSetsSilenceThreshold);

  ofxDatGuiToggle* onSetsUseTimeThresholdSlider = onsets->addToggle("useTimeThreshold", onSetsUseTimeThreshold);

  ofxDatGuiSlider* onSetsTimeThresholdSlider = onsets->addSlider("timeThreshold", 0.0, 1000.0);
  onSetsTimeThresholdSlider->bind(onSetsTimeThreshold);

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
    float rms = audioAnalyzer.getValue(RMS, i, smoothing);

    if (rms > RMSThreshold) {
      string param = "{";

      param += "\"rms\":" + ofToString(rms) + ",";
      float power = audioAnalyzer.getValue(POWER, i, smoothing);
      param += "\"power\":" + ofToString(power) + ",";
      float pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, i);
      param += "\"pitchFreq\":" + ofToString(pitchFreq) + ",";
      float pitchSalience = audioAnalyzer.getValue(PITCH_SALIENCE, i);
      param += "\"pitchSalience\":" + ofToString(pitchSalience) + ",";
      float inharmonicity = audioAnalyzer.getValue(INHARMONICITY, i);
      param += "\"inharmonicity\":" + ofToString(inharmonicity) + ",";
      float centroid = audioAnalyzer.getValue(CENTROID, i);
      param += "\"centroid\":" + ofToString(centroid) + ",";
      float rollOff = audioAnalyzer.getValue(ROLL_OFF, i);
      param += "\"rollOff\":" + ofToString(rollOff) + ",";
      float strongPeak = audioAnalyzer.getValue(STRONG_PEAK, i);
      param += "\"strongPeak\":" + ofToString(strongPeak) + ",";
      float isOnset = audioAnalyzer.getOnsetValue(i);
      param += "\"isOnset\":" + ofToString(isOnset);

      param += "}";

      string eventName = "channel-" + ofToString(i);
      socketIO.emit(eventName, param);
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &inBuffer){
  audioAnalyzer.setOnsetsParameters(0, onSetsAlpha, onSetsSilenceThreshold, onSetsTimeThreshold, onSetsUseTimeThreshold);
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
