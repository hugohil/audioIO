#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetDataPathRoot("../Resources/data/");

  ofSetWindowTitle("AudioIO");
  ofBackground(34, 34, 34);
  ofSetFrameRate(60);

  soundStream.printDeviceList();
  deviceList = soundStream.getDeviceList();

  for (int i = 0; i < arguments.size(); i++){
    if (arguments[i] == "--device") {
      deviceIndex = stoi(arguments[i + 1]);
      device = deviceList[deviceIndex];
    } else if (arguments[i] == "--host") {
      host = arguments[i + 1];
    } else if (arguments[i] == "--port") {
      port = arguments[i + 1];
    } else if (arguments[i] == "--autostart") {
      autostart = true;
    }
  }

  ofxDatGuiLog::quiet();

  networkGUI = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
  networkGUI->addHeader("Network");
  networkGUI->setTheme(new ofxDatGuiThemeFUBAR());

  deviceGUI = new ofxDatGui(ofxDatGuiAnchor::TOP_CENTER);
  deviceGUI->addHeader("Device");
  deviceGUI->setTheme(new ofxDatGuiThemeFUBAR());

  audioGUI = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
  audioGUI->addHeader("Audio");
  audioGUI->setTheme(new ofxDatGuiThemeFUBAR());

  ofxDatGuiSlider* RMSThresholdSlider = audioGUI->addSlider("RMS threshold", 0.0, 1.0);
  RMSThresholdSlider->bind(RMSThreshold);

  ofxDatGuiSlider* smoothingSlider = audioGUI->addSlider("smoothing", 0.0, 1.0);
  smoothingSlider->bind(smoothing);

  // Onsets
  ofxDatGuiFolder* onsets = audioGUI->addFolder("onsets", ofColor::white);

  ofxDatGuiSlider* onSetsAlphaSlider = onsets->addSlider("alpha", 0.0, 1.0);
  onSetsAlphaSlider->bind(onSetsAlpha);

  ofxDatGuiSlider* onSetsSilenceThresholdSlider = onsets->addSlider("silenceThreshold", 0.0, 1.0);
  onSetsSilenceThresholdSlider->bind(onSetsSilenceThreshold);

  ofxDatGuiToggle* onSetsUseTimeThresholdSlider = onsets->addToggle("useTimeThreshold", onSetsUseTimeThreshold);

  ofxDatGuiSlider* onSetsTimeThresholdSlider = onsets->addSlider("timeThreshold", 0.0, 1000.0);
  onSetsTimeThresholdSlider->bind(onSetsTimeThreshold);

  // Devices

  deviceGUI->addDropdown(ofToString(sampleRate), sampleRates)->onDropdownEvent(this, &ofApp::onSampleRateDropdownEvent);
  deviceGUI->addDropdown(ofToString(bufferSize), bufferSizes)->onDropdownEvent(this, &ofApp::onBufferSizeDropdownEvent);

  vector<string> deviceNames;
  for (vector<ofSoundDevice>::iterator device = deviceList.begin(); device != deviceList.end(); ++device) {
    deviceNames.push_back(device->name);
  }
  deviceGUI->addDropdown("devices available", deviceNames)->onDropdownEvent(this, &ofApp::onDevicesDropdownEvent);

  ofxDatGuiSlider* offsetChannelsSlider = deviceGUI->addSlider("offset", 0, (inChannels - 1), offsetChannels);
  offsetChannelsSlider->setPrecision(0);
  offsetChannelsSlider->bind(offsetChannels);

  ofxDatGuiSlider* activeChannelsSlider = deviceGUI->addSlider("active channels", (offsetChannels + 1), inChannels, activeChannels);
  activeChannelsSlider->setPrecision(0);
  activeChannelsSlider->bind(activeChannels);

  startButton = deviceGUI->addButton("start/stop");
  startButton->onButtonEvent(this, &ofApp::onButtonEvent);

  // Network
  hostInput = networkGUI->addTextInput("HOST", host);
  portInput = networkGUI->addTextInput("PORT", port);
  connectButton = networkGUI->addButton("connect/disconnect");
  connectButton->onButtonEvent(this, &ofApp::onButtonEvent);
  ofAddListener(socketIO.connectionEvent, this, &ofApp::onConnection);

  if (autostart) {
    connect();
    setupDevice();
    toggleStream();
  }
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
  if (e.target == connectButton){
    if (socketIO.getStatus() == "closed") {
      connect();
    } else {
      socketIO.closeConnection();
    }
  } else if (e.target == startButton) {
    toggleStream();
  }
}

void ofApp::connect(){
  host = hostInput->getText();
  port = portInput->getText();

  string address = "http://" + ofToString(host) + ":" + ofToString(port);
  socketIO.setup(address);
}

void ofApp::onConnection(){
  string key = "register";
  string message = "{\"clientName\": \"audioIO\"}";
  socketIO.emit(key, message);
}

void ofApp::toggleStream(){
  if (isStreamActive) {
    isStreamActive = false;
    soundStream.stop();
  } else {
    audioAnalyzer.reset(sampleRate, bufferSize, activeChannels);
    soundStream.setup(this, outChannels, activeChannels, sampleRate, bufferSize, 3);
    isStreamActive = true;
  }
  ofLogNotice()<< (isStreamActive ? "-- START STREAM" : "-- STOP STREAM");
}

//--------------------------------------------------------------
void ofApp::onDevicesDropdownEvent(ofxDatGuiDropdownEvent e){
  device = deviceList[e.child];
  setupDevice();
}

void ofApp::setupDevice () {
  // get device config
  outChannels = device.outputChannels;
  inChannels = device.inputChannels;

  activeChannels = (activeChannels > inChannels) ? inChannels : activeChannels;
  deviceGUI->getSlider("active channels")->setValue(activeChannels);
  deviceGUI->getSlider("active channels")->setMax(inChannels);

  deviceGUI->getSlider("offset")->setMax((inChannels - 1));

  soundStream.setDevice(device);
  audioAnalyzer.reset(sampleRate, bufferSize, activeChannels);
}

void ofApp::onSampleRateDropdownEvent(ofxDatGuiDropdownEvent e){
  sampleRate = stof(sampleRates[e.child]);
}
void ofApp::onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e){
  bufferSize = stof(bufferSizes[e.child]);
}

//--------------------------------------------------------------
void ofApp::update(){
  // get the analysis values for every input channel and send it
  if (isStreamActive) {
    for (int i = offsetChannels; i < activeChannels; ++i) {
      // audioAnalyzer.resetOnsets(i);
      audioAnalyzer.setOnsetsParameters(i, onSetsAlpha, onSetsSilenceThreshold, onSetsTimeThreshold, onSetsUseTimeThreshold);
      float rms = audioAnalyzer.getValue(RMS, i, smoothing);

      if (rms > RMSThreshold) {
        string param = "{";

        param += "\"rms\":" + ofToString(rms) + ",";
        float power = audioAnalyzer.getValue(POWER, i, smoothing);
        param += "\"power\":" + ofToString(power) + ",";
        // float pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, i, smoothing);
        // param += "\"pitchFreq\":" + ofToString(pitchFreq) + ",";
        // float pitchSalience = audioAnalyzer.getValue(PITCH_SALIENCE, i);
        // param += "\"pitchSalience\":" + ofToString(pitchSalience) + ",";
        // float inharmonicity = audioAnalyzer.getValue(INHARMONICITY, i);
        // param += "\"inharmonicity\":" + ofToString(inharmonicity) + ",";
        // float centroid = audioAnalyzer.getValue(CENTROID, i, smoothing);
        // param += "\"centroid\":" + ofToString(centroid) + ",";
        // float rollOff = audioAnalyzer.getValue(ROLL_OFF, i);
        // param += "\"rollOff\":" + ofToString(rollOff) + ",";
        // float strongPeak = audioAnalyzer.getValue(STRONG_PEAK, i);
        // param += "\"strongPeak\":" + ofToString(strongPeak) + ",";
        float isOnset = audioAnalyzer.getOnsetValue(i);
        param += "\"isOnset\":" + ofToString(isOnset) + ",";

        param += "\"channel\":" + ofToString(i);
        param += "}";

        if (socketIO.getStatus() == "connected") {
          string eventName = "aio-datas";
          socketIO.emit(eventName, param);
        }
      }
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(0,255,0);
  ofDrawBitmapString("App arguments: ", 20.0f, 20.0f);
  ofSetColor(255);
  for (int i = 0; i < arguments.size(); i++){
    ofDrawBitmapString(arguments[i], 20.0f, 60.0f + (20*i));
  }
}

//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &inBuffer){
  if (isStreamActive) {
    audioAnalyzer.analyze(inBuffer);
  }
}

//--------------------------------------------------------------
void ofApp::exit(){
  isStreamActive = false;
  audioAnalyzer.exit();
  soundStream.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (key == ' ') {
    toggleStream();
  }
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
