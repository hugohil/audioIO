#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetDataPathRoot("../Resources/data/");

  ofSetWindowTitle("AudioIO");
  ofBackground(34, 34, 34);
  ofSetFrameRate(60);

  controls = "[space] to toggle analysis";
  controls += "\n[s] to setup stream and analyzer";
  controls += "\n[n/d/a] to save network/device/audio settings";
  controls += "\n[esc] to quit";

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

  loadSettings();

  ofxDatGuiButton* saveAudioSettingsButton = audioGUI->addButton("save settings");
  saveAudioSettingsButton->onButtonEvent(this, &ofApp::saveAudioSettings);

  ofxDatGuiSlider* RMSThresholdSlider = audioGUI->addSlider("RMS threshold", 0.0, 1.0);
  RMSThresholdSlider->bind(RMSThreshold);

  ofxDatGuiSlider* smoothingSlider = audioGUI->addSlider("smoothing", 0.0, 1.0);
  smoothingSlider->bind(smoothing);

  // Features
  ofxDatGuiFolder* features = audioGUI->addFolder("features", ofColor::white);
  ofxDatGuiToggle* toggleRMS = features->addToggle("rms", featureRMS);
  toggleRMS->onToggleEvent(this, &ofApp::onToggleRMSEvent);
  ofxDatGuiToggle* togglePOWER = features->addToggle("power", featurePOWER);
  togglePOWER->onToggleEvent(this, &ofApp::onTogglePOWEREvent);
  ofxDatGuiToggle* togglePITCH_FREQ = features->addToggle("pitch freq", featurePITCH_FREQ);
  togglePITCH_FREQ->onToggleEvent(this, &ofApp::onTogglePITCH_FREQEvent);
  ofxDatGuiToggle* togglePITCH_SALIENCE = features->addToggle("pitch salience", featurePITCH_SALIENCE);
  togglePITCH_SALIENCE->onToggleEvent(this, &ofApp::onTogglePITCH_SALIENCEEvent);
  ofxDatGuiToggle* toggleINHARMONICITY = features->addToggle("inharmonicity", featureINHARMONICITY);
  toggleINHARMONICITY->onToggleEvent(this, &ofApp::onToggleINHARMONICITYEvent);
  ofxDatGuiToggle* toggleCENTROID = features->addToggle("centroid", featureCENTROID);
  toggleCENTROID->onToggleEvent(this, &ofApp::onToggleCENTROIDEvent);
  ofxDatGuiToggle* toggleROLL_OFF = features->addToggle("roll off", featureROLL_OFF);
  toggleROLL_OFF->onToggleEvent(this, &ofApp::onToggleROLL_OFFEvent);
  ofxDatGuiToggle* toggleSTRONG_PEAK = features->addToggle("strong peak", featureSTRONG_PEAK);
  toggleSTRONG_PEAK->onToggleEvent(this, &ofApp::onToggleSTRONG_PEAKEvent);
  ofxDatGuiToggle* toggleONSETS = features->addToggle("onsets", featureONSETS);
  toggleONSETS->onToggleEvent(this, &ofApp::onToggleONSETSEvent);
  // Onsets
  ofxDatGuiFolder* onsets = audioGUI->addFolder("onsets", ofColor::white);

  ofxDatGuiSlider* onSetsAlphaSlider = onsets->addSlider("alpha", 0.0, 1.0);
  onSetsAlphaSlider->bind(onSetsAlpha);

  ofxDatGuiSlider* onSetsSilenceThresholdSlider = onsets->addSlider("silenceThreshold", 0.0, 1.0);
  onSetsSilenceThresholdSlider->bind(onSetsSilenceThreshold);

  ofxDatGuiToggle* onSetsUseTimeThreshold = onsets->addToggle("useTimeThreshold", onSetsUseTimeThreshold);
  onSetsUseTimeThreshold->onToggleEvent(this, &ofApp::onSetsUseTimeThresholdEvent);

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

  ofxDatGuiSlider* offsetChannelsSlider = deviceGUI->addSlider("offset", 0, (inChannels - 1));
  offsetChannelsSlider->setPrecision(0);
  offsetChannelsSlider->setValue(int(offsetChannels));
  offsetChannelsSlider->onSliderEvent(this, &ofApp::onOffsetChannelsSliderEvent);

  ofxDatGuiSlider* activeChannelsSlider = deviceGUI->addSlider("active channels", (offsetChannels + 1), inChannels);
  activeChannelsSlider->setPrecision(0);
  activeChannelsSlider->setValue(int(activeChannels));
  activeChannelsSlider->onSliderEvent(this, &ofApp::onActiveChannelsSliderEvent);

  ofxDatGuiButton* setupButton = deviceGUI->addButton("setup device and analyzer");
  setupButton->onButtonEvent(this, &ofApp::setupAnalyzer);

  startButton = deviceGUI->addButton("toggle analysis");
  startButton->onButtonEvent(this, &ofApp::onButtonEvent);

  // Network
  ofxDatGuiButton* saveNetworkSettingsButton = networkGUI->addButton("save settings");
  saveNetworkSettingsButton->onButtonEvent(this, &ofApp::saveNetworkSettings);

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
  } else {
    isStreamActive = true;
  }
  ofLogNotice()<< (isStreamActive ? "-- START STREAM" : "-- STOP STREAM");
}

void ofApp::setupAnalyzer(ofxDatGuiButtonEvent e){
  setupAnalyzer();
}

void ofApp::setupAnalyzer(){
  soundStream.stop();
  soundStream.close();
  soundStream.setup(this, outChannels, activeChannels, sampleRate, bufferSize, 3);
  audioAnalyzer.reset(sampleRate, bufferSize, activeChannels);
}

//--------------------------------------------------------------
void ofApp::onDevicesDropdownEvent(ofxDatGuiDropdownEvent e){
  deviceIndex = e.child;
  device = deviceList[deviceIndex];
  setupDevice();
  setupAnalyzer();
}

void ofApp::setupDevice () {
  soundStream.stop();
  soundStream.close();
  // get device config
  outChannels = device.outputChannels;
  inChannels = device.inputChannels;

  activeChannels = (activeChannels > inChannels) ? inChannels : activeChannels;
  deviceGUI->getSlider("active channels")->setValue(activeChannels);
  deviceGUI->getSlider("active channels")->setMax(inChannels);

  offsetChannels = (offsetChannels > (inChannels - 1)) ? (inChannels - 1) : offsetChannels;
  deviceGUI->getSlider("offset")->setMax((inChannels - 1));

  resetDebugChannels();
  soundStream.setDevice(device);
}

void ofApp::onSampleRateDropdownEvent(ofxDatGuiDropdownEvent e){
  sampleRate = stof(sampleRates[e.child]);
}
void ofApp::onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e){
  bufferSize = stof(bufferSizes[e.child]);
}

void ofApp::onToggleRMSEvent(ofxDatGuiToggleEvent e){
  featureRMS = e.checked;
}
void ofApp::onTogglePOWEREvent(ofxDatGuiToggleEvent e){
  featurePOWER = e.checked;
}
void ofApp::onTogglePITCH_FREQEvent(ofxDatGuiToggleEvent e){
  featurePITCH_FREQ = e.checked;
}
void ofApp::onTogglePITCH_SALIENCEEvent(ofxDatGuiToggleEvent e){
  featurePITCH_SALIENCE = e.checked;
}
void ofApp::onToggleINHARMONICITYEvent(ofxDatGuiToggleEvent e){
  featureINHARMONICITY = e.checked;
}
void ofApp::onToggleCENTROIDEvent(ofxDatGuiToggleEvent e){
  featureCENTROID = e.checked;
}
void ofApp::onToggleROLL_OFFEvent(ofxDatGuiToggleEvent e){
  featureROLL_OFF = e.checked;
}
void ofApp::onToggleSTRONG_PEAKEvent(ofxDatGuiToggleEvent e){
  featureSTRONG_PEAK = e.checked;
}
void ofApp::onToggleONSETSEvent(ofxDatGuiToggleEvent e){
  featureONSETS = e.checked;
}

void ofApp::onSetsUseTimeThresholdEvent(ofxDatGuiToggleEvent e){
  onSetsUseTimeThreshold = e.checked;
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
  smoothing = settings.getValue("audio:smoothing", smoothing);
  featureRMS = (bool) settings.getValue("audio:featureRMS", featureRMS);
  featurePOWER = (bool) settings.getValue("audio:featurePOWER", featurePOWER);
  featurePITCH_FREQ = (bool) settings.getValue("audio:featurePITCH_FREQ", featurePITCH_FREQ);
  featurePITCH_SALIENCE = (bool) settings.getValue("audio:featurePITCH_SALIENCE", featurePITCH_SALIENCE);
  featureINHARMONICITY = (bool) settings.getValue("audio:featureINHARMONICITY", featureINHARMONICITY);
  featureCENTROID = (bool) settings.getValue("audio:featureCENTROID", featureCENTROID);
  featureROLL_OFF = (bool) settings.getValue("audio:featureROLL_OFF", featureROLL_OFF);
  featureSTRONG_PEAK = (bool) settings.getValue("audio:featureSTRONG_PEAK", featureSTRONG_PEAK);
  featureONSETS = (bool) settings.getValue("audio:featureONSETS", featureONSETS);
  onSetsAlpha = settings.getValue("audio:onSetsAlpha", onSetsAlpha);
  onSetsSilenceThreshold = settings.getValue("audio:onSetsSilenceThreshold", onSetsSilenceThreshold);
  onSetsUseTimeThreshold = settings.getValue("audio:onSetsUseTimeThreshold", onSetsUseTimeThreshold);
  onSetsTimeThreshold = settings.getValue("audio:onSetsTimeThreshold", onSetsTimeThreshold);

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
  setupAnalyzer();
}

void ofApp::saveNetworkSettings(ofxDatGuiButtonEvent e){
  saveNetworkSettings();
}
void ofApp::saveNetworkSettings(){
  settings.setValue("network:host", host);
  settings.setValue("network:port", port);
  settings.saveFile("settings.xml");
}

void ofApp::saveDeviceSettings(ofxDatGuiButtonEvent e){
  saveDeviceSettings();
}
void ofApp::saveDeviceSettings(){
  settings.setValue("device:deviceIndex", deviceIndex);
  settings.setValue("device:sampleRate", sampleRate);
  settings.setValue("device:bufferSize", bufferSize);
  settings.setValue("device:offsetChannels", offsetChannels);
  settings.setValue("device:activeChannels", activeChannels);
  settings.saveFile("settings.xml");
}

void ofApp::saveAudioSettings(ofxDatGuiButtonEvent e){
  saveAudioSettings();
}
void ofApp::saveAudioSettings(){
  settings.setValue("audio:RMSThreshold", RMSThreshold);
  settings.setValue("audio:smoothing", smoothing);
  settings.setValue("audio:featureRMS", featureRMS);
  settings.setValue("audio:featurePOWER", featurePOWER);
  settings.setValue("audio:featurePITCH_FREQ", featurePITCH_FREQ);
  settings.setValue("audio:featurePITCH_SALIENCE", featurePITCH_SALIENCE);
  settings.setValue("audio:featureINHARMONICITY", featureINHARMONICITY);
  settings.setValue("audio:featureCENTROID", featureCENTROID);
  settings.setValue("audio:featureROLL_OFF", featureROLL_OFF);
  settings.setValue("audio:featureSTRONG_PEAK", featureSTRONG_PEAK);
  settings.setValue("audio:featureONSETS", featureONSETS);
  settings.setValue("audio:onSetsAlpha", onSetsAlpha);
  settings.setValue("audio:onSetsSilenceThreshold", onSetsSilenceThreshold);
  settings.setValue("audio:onSetsUseTimeThreshold", onSetsUseTimeThreshold);
  settings.setValue("audio:onSetsTimeThreshold", onSetsTimeThreshold);
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

        if (featureRMS) {
          param += "\"rms\":" + ofToString(rms, 4) + ",";
        }
        if (featurePOWER) {
          float power = audioAnalyzer.getValue(POWER, i, smoothing);
          param += "\"power\":" + ofToString(power, 4) + ",";
        }
        if (featurePITCH_FREQ) {
          float pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, i, smoothing);
          param += "\"pitchFreq\":" + ofToString(pitchFreq) + ",";
        }
        if (featurePITCH_SALIENCE) {
          float pitchSalience = audioAnalyzer.getValue(PITCH_SALIENCE, i);
          param += "\"pitchSalience\":" + ofToString(pitchSalience) + ",";
        }
        if (featureINHARMONICITY) {
          float inharmonicity = audioAnalyzer.getValue(INHARMONICITY, i);
          param += "\"inharmonicity\":" + ofToString(inharmonicity) + ",";
        }
        if (featureCENTROID) {
          float centroid = audioAnalyzer.getValue(CENTROID, i, smoothing);
          param += "\"centroid\":" + ofToString(centroid) + ",";
        }
        if (featureROLL_OFF) {
          float rollOff = audioAnalyzer.getValue(ROLL_OFF, i);
          param += "\"rollOff\":" + ofToString(rollOff) + ",";
        }
        if (featureSTRONG_PEAK) {
          float strongPeak = audioAnalyzer.getValue(STRONG_PEAK, i);
          param += "\"strongPeak\":" + ofToString(strongPeak) + ",";
        }
        if (featureONSETS) {
          float isOnset = audioAnalyzer.getOnsetValue(i);
          param += "\"isOnset\":" + ofToString(isOnset, 4) + ",";
        }

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
      ofDrawBitmapString(debug, ((debugWidth * i) + 10.0f), (ofGetHeight() - 40.0f));
    }
  }
  ofSetColor(255);
  string streamStatus = (isStreamActive ? "STARTED" : "STOPPED");
  status = "STATUS:\n";
  status += "\nnetwork: " + socketIO.getStatus();
  status += "\ndevice: " + device.name;
  status += "\nstream: " + streamStatus;
  ofDrawBitmapString(status, 20.0f, ofGetHeight() - 180.0f);
  ofSetColor(125);
  ofDrawBitmapString(controls, 20.0f, ofGetHeight() - 100.0f);
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
  } else if (key == 's') {
    setupAnalyzer();
  } else if (key == 'n') {
    saveNetworkSettings();
  } else if (key == 'd') {
    saveDeviceSettings();
  } else if (key == 'a') {
    saveAudioSettings();
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
