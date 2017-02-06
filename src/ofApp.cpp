#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(34, 34, 34);
  ofSetFrameRate(60);

  // ofxDatGuiLog::quiet();

  ofTrueTypeFont::setGlobalDpi(72);
  verdana.load("gui_assets/fonts/verdana.ttf", 22, true, true);

  sender.setup(HOST, PORT);

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

//--------------------------------------------------------------
void ofApp::onDevicesDropdownEvent(ofxDatGuiDropdownEvent e){
  soundStream.stop();
  audioAnalyzer.exit();

  ofSoundDevice device = deviceList[e.child];

  // get device config
  outChannels = device.outputChannels;
  inChannels = device.inputChannels;

  for (int i = 0; i < inChannels; ++i) {
    Channel channel;
    channel.numChannel = i;
    channels.insert(channels.begin() + i, channel);
  }

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
      ofxOscBundle bundle;

      channels[i].rms = rms;
      ofxOscMessage messageRms = channels[i].getMessageRms();
      bundle.addMessage(messageRms);

      channels[i].power = audioAnalyzer.getValue(POWER, 0);
      ofxOscMessage messagePower = channels[i].getMessagePower();
      bundle.addMessage(messagePower);

      channels[i].pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, 0);
      ofxOscMessage messagePitchFreq = channels[i].getMessagePitchFreq();
      bundle.addMessage(messagePitchFreq);

      channels[i].pitchConf = audioAnalyzer.getValue(PITCH_CONFIDENCE, 0);
      ofxOscMessage messagePitchConf = channels[i].getMessagePitchConf();
      bundle.addMessage(messagePitchConf);

      channels[i].pitchSalience = audioAnalyzer.getValue(PITCH_SALIENCE, 0);
      ofxOscMessage messagePitchSalience = channels[i].getMessagePitchSalience();
      bundle.addMessage(messagePitchSalience);

      channels[i].inharmonicity = audioAnalyzer.getValue(INHARMONICITY, 0);
      ofxOscMessage messageInharmonicity = channels[i].getMessageInharmonicity();
      bundle.addMessage(messageInharmonicity);

      channels[i].hfc = audioAnalyzer.getValue(HFC, 0);
      ofxOscMessage messageHfc = channels[i].getMessageHfc();
      bundle.addMessage(messageHfc);

      channels[i].specComp = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0);
      ofxOscMessage messageSpecComp = channels[i].getMessageSpecComp();
      bundle.addMessage(messageSpecComp);

      channels[i].centroid = audioAnalyzer.getValue(CENTROID, 0);
      ofxOscMessage messageCentroid = channels[i].getMessageCentroid();
      bundle.addMessage(messageCentroid);

      channels[i].rollOff = audioAnalyzer.getValue(ROLL_OFF, 0);
      ofxOscMessage messageRollOff = channels[i].getMessageRollOff();
      bundle.addMessage(messageRollOff);

      channels[i].oddToEven = audioAnalyzer.getValue(ODD_TO_EVEN, 0);
      ofxOscMessage messageOddToEven = channels[i].getMessageOddToEven();
      bundle.addMessage(messageOddToEven);

      channels[i].strongPeak = audioAnalyzer.getValue(STRONG_PEAK, 0);
      ofxOscMessage messageStrongPeak = channels[i].getMessageStrongPeak();
      bundle.addMessage(messageStrongPeak);

      channels[i].strongDecay = audioAnalyzer.getValue(STRONG_DECAY, 0);
      ofxOscMessage messageStrongDecay = channels[i].getMessageStrongDecay();
      bundle.addMessage(messageStrongDecay);

      channels[i].pitchFreqNorm = audioAnalyzer.getValue(PITCH_FREQ, 0);
      ofxOscMessage messagePitchFreqNorm = channels[i].getMessagePitchFreqNorm();
      bundle.addMessage(messagePitchFreqNorm);

      channels[i].hfcNorm = audioAnalyzer.getValue(HFC, 0);
      ofxOscMessage messageHfcNorm = channels[i].getMessageHfcNorm();
      bundle.addMessage(messageHfcNorm);

      channels[i].specCompNorm = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0);
      ofxOscMessage messageSpecCompNorm = channels[i].getMessageSpecCompNorm();
      bundle.addMessage(messageSpecCompNorm);

      channels[i].centroidNorm = audioAnalyzer.getValue(CENTROID, 0);
      ofxOscMessage messageCentroidNorm = channels[i].getMessageCentroidNorm();
      bundle.addMessage(messageCentroidNorm);

      channels[i].rollOffNorm = audioAnalyzer.getValue(ROLL_OFF, 0);
      ofxOscMessage messageRollOffNorm = channels[i].getMessageRollOffNorm();
      bundle.addMessage(messageRollOffNorm);

      channels[i].oddToEvenNorm = audioAnalyzer.getValue(ODD_TO_EVEN, 0);
      ofxOscMessage messageOddToEvenNorm = channels[i].getMessageOddToEvenNorm();
      bundle.addMessage(messageOddToEvenNorm);

      channels[i].strongPeakNorm = audioAnalyzer.getValue(STRONG_PEAK, 0);
      ofxOscMessage messageStrongPeakNorm = channels[i].getMessageStrongPeakNorm();
      bundle.addMessage(messageStrongPeakNorm);

      channels[i].strongDecayNorm = audioAnalyzer.getValue(STRONG_DECAY, 0);
      ofxOscMessage messageStrongDecayNorm = channels[i].getMessageStrongDecayNorm();
      bundle.addMessage(messageStrongDecayNorm);

      channels[i].dissonance = audioAnalyzer.getValue(DISSONANCE, 0);
      ofxOscMessage messageDissonance = channels[i].getMessageDissonance();
      bundle.addMessage(messageDissonance);

      sender.sendBundle(bundle);

      // Those are vectors. Will see later how to send them through OSC.
      // channels[i].spectrum = audioAnalyzer.getValues(SPECTRUM, 0);
      // channels[i].melBands = audioAnalyzer.getValues(MEL_BANDS, 0);
      // channels[i].mfcc = audioAnalyzer.getValues(MFCC, 0);
      // channels[i].hpcp = audioAnalyzer.getValues(HPCP, 0);
      // channels[i].tristimulus = audioAnalyzer.getValues(TRISTIMULUS, 0);
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
