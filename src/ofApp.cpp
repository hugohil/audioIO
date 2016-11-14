#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(34, 34, 34);
  ofSetFrameRate(60);

  soundStream.printDeviceList();
  deviceList = soundStream.getDeviceList();

  sampleRate = 44100;
  bufferSize = 512;

  audioSetupGUI = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  audioSetupGUI->addHeader("DEVICE CONFIG");
  audioSetupGUI->addFooter();
  audioSetupGUI->addFRM();

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

  // setup the sound stream
  soundStream.setDevice(device);
  soundStream.setup(this, outChannels, inChannels, sampleRate, bufferSize, 3);
  // setup ofxAudioAnalyzer with the SAME PARAMETERS
  audioAnalyzer.setup(sampleRate, bufferSize, inChannels);
}


//--------------------------------------------------------------
void ofApp::update(){
  //get the analysis values for every input channel
  for (int i = 0; i < inChannels; ++i) {
    RMSs.insert(RMSs.begin() + i, audioAnalyzer.getValue(RMS, i));
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  // visualize every input channel RMS
  ofDrawBitmapString(ofToString(RMSs.size()), 500, 300);
  for (int i = 0; i < inChannels; ++i) {
    ofDrawBitmapString(ofToString(i) + " -> " + ofToString(RMSs[i]), 10, 12 * (i + 1));
  }
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &inBuffer){
  //ANALYZE SOUNDBUFFER:
  audioAnalyzer.analyze(inBuffer);
}

//--------------------------------------------------------------
void ofApp::exit(){
  ofSoundStreamStop();
  audioAnalyzer.exit();
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
