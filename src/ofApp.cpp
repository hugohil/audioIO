#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  ofBackground(34, 34, 34);
  ofSetFrameRate(60);

  sender.setup(HOST, PORT);

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

  for (int i = 0; i < inChannels; ++i) {
    Channel channel;
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
  // get the analysis values for every input channel and send it
  for (int i = 0; i < inChannels; ++i) {
    bool onset = audioAnalyzer.getOnsetValue(0);

    if (onset) {
      ofxOscBundle bundle;

      channels[i].isOnset = onset;
      ofxOscMessage messageIsOnset = channels[i].getMessageIsOnset(i);
      bundle.addMessage(messageIsOnset);

      channels[i].rms = audioAnalyzer.getValue(RMS, 0);
      ofxOscMessage messageRms = channels[i].getMessageRms(i);
      bundle.addMessage(messageRms);

      channels[i].power = audioAnalyzer.getValue(POWER, 0);
      ofxOscMessage messagePower = channels[i].getMessagePower(i);
      bundle.addMessage(messagePower);

      channels[i].pitchFreq = audioAnalyzer.getValue(PITCH_FREQ, 0);
      ofxOscMessage messagePitchFreq = channels[i].getMessagePitchFreq(i);
      bundle.addMessage(messagePitchFreq);

      channels[i].pitchConf = audioAnalyzer.getValue(PITCH_CONFIDENCE, 0);
      ofxOscMessage messagePitchConf = channels[i].getMessagePitchConf(i);
      bundle.addMessage(messagePitchConf);

      channels[i].pitchSalience = audioAnalyzer.getValue(PITCH_SALIENCE, 0);
      ofxOscMessage messagePitchSalience = channels[i].getMessagePitchSalience(i);
      bundle.addMessage(messagePitchSalience);

      channels[i].inharmonicity = audioAnalyzer.getValue(INHARMONICITY, 0);
      ofxOscMessage messageInharmonicity = channels[i].getMessageInharmonicity(i);
      bundle.addMessage(messageInharmonicity);

      channels[i].hfc = audioAnalyzer.getValue(HFC, 0);
      ofxOscMessage messageHfc = channels[i].getMessageHfc(i);
      bundle.addMessage(messageHfc);

      channels[i].specComp = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0);
      ofxOscMessage messageSpecComp = channels[i].getMessageSpecComp(i);
      bundle.addMessage(messageSpecComp);

      channels[i].centroid = audioAnalyzer.getValue(CENTROID, 0);
      ofxOscMessage messageCentroid = channels[i].getMessageCentroid(i);
      bundle.addMessage(messageCentroid);

      channels[i].rollOff = audioAnalyzer.getValue(ROLL_OFF, 0);
      ofxOscMessage messageRollOff = channels[i].getMessageRollOff(i);
      bundle.addMessage(messageRollOff);

      channels[i].oddToEven = audioAnalyzer.getValue(ODD_TO_EVEN, 0);
      ofxOscMessage messageOddToEven = channels[i].getMessageOddToEven(i);
      bundle.addMessage(messageOddToEven);

      channels[i].strongPeak = audioAnalyzer.getValue(STRONG_PEAK, 0);
      ofxOscMessage messageStrongPeak = channels[i].getMessageStrongPeak(i);
      bundle.addMessage(messageStrongPeak);

      channels[i].strongDecay = audioAnalyzer.getValue(STRONG_DECAY, 0);
      ofxOscMessage messageStrongDecay = channels[i].getMessageStrongDecay(i);
      bundle.addMessage(messageStrongDecay);

      channels[i].pitchFreqNorm = audioAnalyzer.getValue(PITCH_FREQ, 0);
      ofxOscMessage messagePitchFreqNorm = channels[i].getMessagePitchFreqNorm(i);
      bundle.addMessage(messagePitchFreqNorm);

      channels[i].hfcNorm = audioAnalyzer.getValue(HFC, 0);
      ofxOscMessage messageHfcNorm = channels[i].getMessageHfcNorm(i);
      bundle.addMessage(messageHfcNorm);

      channels[i].specCompNorm = audioAnalyzer.getValue(SPECTRAL_COMPLEXITY, 0);
      ofxOscMessage messageSpecCompNorm = channels[i].getMessageSpecCompNorm(i);
      bundle.addMessage(messageSpecCompNorm);

      channels[i].centroidNorm = audioAnalyzer.getValue(CENTROID, 0);
      ofxOscMessage messageCentroidNorm = channels[i].getMessageCentroidNorm(i);
      bundle.addMessage(messageCentroidNorm);

      channels[i].rollOffNorm = audioAnalyzer.getValue(ROLL_OFF, 0);
      ofxOscMessage messageRollOffNorm = channels[i].getMessageRollOffNorm(i);
      bundle.addMessage(messageRollOffNorm);

      channels[i].oddToEvenNorm = audioAnalyzer.getValue(ODD_TO_EVEN, 0);
      ofxOscMessage messageOddToEvenNorm = channels[i].getMessageOddToEvenNorm(i);
      bundle.addMessage(messageOddToEvenNorm);

      channels[i].strongPeakNorm = audioAnalyzer.getValue(STRONG_PEAK, 0);
      ofxOscMessage messageStrongPeakNorm = channels[i].getMessageStrongPeakNorm(i);
      bundle.addMessage(messageStrongPeakNorm);

      channels[i].strongDecayNorm = audioAnalyzer.getValue(STRONG_DECAY, 0);
      ofxOscMessage messageStrongDecayNorm = channels[i].getMessageStrongDecayNorm(i);
      bundle.addMessage(messageStrongDecayNorm);

      channels[i].dissonance = audioAnalyzer.getValue(DISSONANCE, 0);
      ofxOscMessage messageDissonance = channels[i].getMessageDissonance(i);
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
