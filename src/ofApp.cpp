#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetDataPathRoot("../Resources/data/");

  ofSetWindowTitle("AudioIO");
  ofBackground(34, 34, 34);
  ofSetFrameRate(60);

  soundStream.printDeviceList();
  deviceList = soundStream.getDeviceList();

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

  ofxDatGuiButton* saveAudioSettingsButton = audioGUI->addButton("save settings");
  saveAudioSettingsButton->onButtonEvent(this, &ofApp::saveAudioSettings);

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
  ofxDatGuiButton* saveDeviceSettingsButton = deviceGUI->addButton("save settings");
  saveDeviceSettingsButton->onButtonEvent(this, &ofApp::saveDeviceSettings);

  deviceGUI->addDropdown(ofToString(sampleRate), sampleRates)->onDropdownEvent(this, &ofApp::onSampleRateDropdownEvent);
  deviceGUI->addDropdown(ofToString(bufferSize), bufferSizes)->onDropdownEvent(this, &ofApp::onBufferSizeDropdownEvent);

  vector<string> deviceNames;
  for (vector<ofSoundDevice>::iterator device = deviceList.begin(); device != deviceList.end(); ++device) {
    deviceNames.push_back(device->name);
  }
  deviceGUI->addDropdown("devices available", deviceNames)->onDropdownEvent(this, &ofApp::onDevicesDropdownEvent);

  ofxDatGuiSlider* offsetChannelsSlider = deviceGUI->addSlider("offset", 0, (inChannels - 1), offsetChannels);
  offsetChannelsSlider->setPrecision(0);
  offsetChannelsSlider->onSliderEvent(this, &ofApp::onOffsetChannelsSliderEvent);

  ofxDatGuiSlider* activeChannelsSlider = deviceGUI->addSlider("active channels", (offsetChannels + 1), inChannels, activeChannels);
  activeChannelsSlider->setPrecision(0);
  activeChannelsSlider->onSliderEvent(this, &ofApp::onActiveChannelsSliderEvent);

  startButton = deviceGUI->addButton("start/stop");
  startButton->onButtonEvent(this, &ofApp::onButtonEvent);

  // Network
  ofxDatGuiButton* saveNetworkSettingsButton = networkGUI->addButton("save settings");
  saveNetworkSettingsButton->onButtonEvent(this, &ofApp::saveNetworkSettings);

  hostInput = networkGUI->addTextInput("HOST", host);
  portInput = networkGUI->addTextInput("PORT", port);
  connectButton = networkGUI->addButton("connect/disconnect");
  connectButton->onButtonEvent(this, &ofApp::onButtonEvent);
  ofAddListener(socketIO.connectionEvent, this, &ofApp::onConnection);

  loadSettings();

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

void ofApp::onOffsetChannelsSliderEvent(ofxDatGuiSliderEvent e){
  offsetChannels = e.value;
  resetDebugChannels();
}

void ofApp::onActiveChannelsSliderEvent(ofxDatGuiSliderEvent e){
  activeChannels = e.value;
  resetDebugChannels();
}

void ofApp::resetDebugChannels(){
  debugChannels.clear();
  for (int i = offsetChannels; i < activeChannels; ++i) {
    debugChannels.push_back(0.0f);
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
  deviceIndex = e.child;
  device = deviceList[deviceIndex];
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

void ofApp::loadSettings(){
  settings.loadFile("settings.xml");

  host = settings.getValue("network:host", host);
  port = settings.getValue("network:port", port);

  deviceIndex = settings.getValue("device:deviceIndex", deviceIndex);
  sampleRate = settings.getValue("device:sampleRate", sampleRate);
  bufferSize = settings.getValue("device:bufferSize", bufferSize);
  offsetChannels = settings.getValue("device:offsetChannels", offsetChannels);
  activeChannels = settings.getValue("device:activeChannels", activeChannels);

  RMSThreshold = settings.getValue("audio:RMSThreshold", RMSThreshold);
  onSetsAlpha = settings.getValue("audio:onSetsAlpha", onSetsAlpha);
  onSetsSilenceThreshold = settings.getValue("audio:onSetsSilenceThreshold", onSetsSilenceThreshold);
  onSetsUseTimeThreshold = settings.getValue("audio:onSetsUseTimeThreshold", onSetsUseTimeThreshold);
  onSetsTimeThreshold = settings.getValue("audio:onSetsTimeThreshold", onSetsTimeThreshold);
  smoothing = settings.getValue("audio:smoothing", smoothing);

  for (int i = 0; i < arguments.size(); i++){
    if (arguments[i] == "--device") {
      deviceIndex = stoi(arguments[i + 1]);
    } else if (arguments[i] == "--host") {
      host = arguments[i + 1];
    } else if (arguments[i] == "--port") {
      port = arguments[i + 1];
    } else if (arguments[i] == "--autostart") {
      autostart = true;
    }
  }

  device = deviceList[deviceIndex];
  setupDevice();
}

void ofApp::saveNetworkSettings(ofxDatGuiButtonEvent e){
  settings.setValue("network:host", host);
  settings.setValue("network:port", port);
  settings.saveFile("settings.xml");
}

void ofApp::saveDeviceSettings(ofxDatGuiButtonEvent e){
  settings.setValue("device:deviceIndex", deviceIndex);
  settings.setValue("device:sampleRate", sampleRate);
  settings.setValue("device:bufferSize", bufferSize);
  settings.setValue("device:offsetChannels", offsetChannels);
  settings.setValue("device:activeChannels", activeChannels);
  settings.saveFile("settings.xml");
}

void ofApp::saveAudioSettings(ofxDatGuiButtonEvent e){
  settings.setValue("audio:RMSThreshold", RMSThreshold);
  settings.setValue("audio:onSetsAlpha", onSetsAlpha);
  settings.setValue("audio:onSetsSilenceThreshold", onSetsSilenceThreshold);
  settings.setValue("audio:onSetsUseTimeThreshold", onSetsUseTimeThreshold);
  settings.setValue("audio:onSetsTimeThreshold", onSetsTimeThreshold);
  settings.setValue("audio:smoothing", smoothing);
  settings.saveFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update(){
  // get the analysis values for every input channel and send it
  if (isStreamActive) {
    for (int i = offsetChannels; i < activeChannels; ++i) {
      // audioAnalyzer.resetOnsets(i);
      audioAnalyzer.setOnsetsParameters(i, onSetsAlpha, onSetsSilenceThreshold, onSetsTimeThreshold, onSetsUseTimeThreshold);
      float rms = audioAnalyzer.getValue(RMS, i, smoothing);
      debugChannels[(i - offsetChannels)] = rms;

      if (rms > RMSThreshold) {
        string param = "{";

        param += "\"rms\":" + ofToString(rms, 4) + ",";
        float power = audioAnalyzer.getValue(POWER, i, smoothing);
        param += "\"power\":" + ofToString(power, 4) + ",";
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
        param += "\"isOnset\":" + ofToString(isOnset, 4) + ",";

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
  if (isStreamActive) {
    float debugWidth = (ofGetWidth() / debugChannels.size());
    for (int i = 0; i < debugChannels.size(); ++i){
      ofSetColor(200);
      ofDrawRectangle((i * debugWidth), ofGetHeight(), debugWidth, -(debugChannels[i] * ofGetHeight()));
      ofSetColor(255);
      string debug = "channel #" + ofToString(i + offsetChannels) + "\nRMS: " + ofToString(debugChannels[i], 4);
      ofDrawBitmapString(debug, ((debugWidth * i) + 5.0f), (ofGetHeight() - 20.0f));
    }
  }
  ofSetColor(255);
  string streamStatus = (isStreamActive ? "STARTED" : "STOPPED");
  string status = "STATUS:\n";
  status += "\nnetwork: " + socketIO.getStatus();
  status += "\ndevice: " + device.name;
  status += "\nstream: " + streamStatus;
  ofDrawBitmapString(status, 20.0f, ofGetHeight() - 140.0f);
  ofSetColor(125);
  ofDrawBitmapString("note: restarting stream does not work. you need to restart the app.", 20.0f, ofGetHeight() - 60.0f);
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
