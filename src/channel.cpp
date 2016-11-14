#include "channel.h"

ofxOscMessage Channel::getMessageRms (int numChannel) {
  // float rms;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/rms");
  m.addFloatArg(rms);
  return m;
}

ofxOscMessage Channel::getMessagePower (int numChannel) {
  // float power;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/power");
  m.addFloatArg(power);
  return m;
}

ofxOscMessage Channel::getMessagePitchFreq (int numChannel) {
  // float pitchFreq;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/pitchFreq");
  m.addFloatArg(pitchFreq);
  return m;
}

ofxOscMessage Channel::getMessagePitchFreqNorm (int numChannel) {
  // float pitchFreqNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/pitchFreqNorm");
  m.addFloatArg(pitchFreqNorm);
  return m;
}

ofxOscMessage Channel::getMessagePitchConf (int numChannel) {
  // float pitchConf;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/pitchConf");
  m.addFloatArg(pitchConf);
  return m;
}

ofxOscMessage Channel::getMessagePitchSalience (int numChannel) {
  // float pitchSalience;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/pitchSalience");
  m.addFloatArg(pitchSalience);
  return m;
}

ofxOscMessage Channel::getMessageHfc (int numChannel) {
  // float hfc;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/hfc");
  m.addFloatArg(hfc);
  return m;
}

ofxOscMessage Channel::getMessageHfcNorm (int numChannel) {
  // float hfcNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/hfcNorm");
  m.addFloatArg(hfcNorm);
  return m;
}

ofxOscMessage Channel::getMessageSpecComp (int numChannel) {
  // float specComp;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/specComp");
  m.addFloatArg(specComp);
  return m;
}

ofxOscMessage Channel::getMessageSpecCompNorm (int numChannel) {
  // float specCompNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/specCompNorm");
  m.addFloatArg(specCompNorm);
  return m;
}

ofxOscMessage Channel::getMessageCentroid (int numChannel) {
  // float centroid;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/centroid");
  m.addFloatArg(centroid);
  return m;
}

ofxOscMessage Channel::getMessageCentroidNorm (int numChannel) {
  // float centroidNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/centroidNorm");
  m.addFloatArg(centroidNorm);
  return m;
}

ofxOscMessage Channel::getMessageInharmonicity (int numChannel) {
  // float inharmonicity;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/inharmonicity");
  m.addFloatArg(inharmonicity);
  return m;
}

ofxOscMessage Channel::getMessageDissonance (int numChannel) {
  // float dissonance;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/dissonance");
  m.addFloatArg(dissonance);
  return m;
}

ofxOscMessage Channel::getMessageRollOff (int numChannel) {
  // float rollOff;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/rollOff");
  m.addFloatArg(rollOff);
  return m;
}

ofxOscMessage Channel::getMessageRollOffNorm (int numChannel) {
  // float rollOffNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/rollOffNorm");
  m.addFloatArg(rollOffNorm);
  return m;
}

ofxOscMessage Channel::getMessageOddToEven (int numChannel) {
  // float oddToEven;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/oddToEven");
  m.addFloatArg(oddToEven);
  return m;
}

ofxOscMessage Channel::getMessageOddToEvenNorm (int numChannel) {
  // float oddToEvenNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/oddToEvenNorm");
  m.addFloatArg(oddToEvenNorm);
  return m;
}

ofxOscMessage Channel::getMessageStrongPeak (int numChannel) {
  // float strongPeak;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/strongPeak");
  m.addFloatArg(strongPeak);
  return m;
}

ofxOscMessage Channel::getMessageStrongPeakNorm (int numChannel) {
  // float strongPeakNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/strongPeakNorm");
  m.addFloatArg(strongPeakNorm);
  return m;
}

ofxOscMessage Channel::getMessageStrongDecay (int numChannel) {
  // float strongDecay;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/strongDecay");
  m.addFloatArg(strongDecay);
  return m;
}

ofxOscMessage Channel::getMessageStrongDecayNorm (int numChannel) {
  // float strongDecayNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/strongDecayNorm");
  m.addFloatArg(strongDecayNorm);
  return m;
}

// ofxOscMessage Channel::getMessageSpectrum (int numChannel) {
//   // vector<float> spectrum;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel) + "/spectrum");
//   m.addFloatArg(spectrum);
//   return m;
// }

// ofxOscMessage Channel::getMessageMelBands (int numChannel) {
//   // vector<float> melBands;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel) + "/melBands");
//   m.addFloatArg(melBands);
//   return m;
// }

// ofxOscMessage Channel::getMessageMfcc (int numChannel) {
//   // vector<float> mfcc;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel) + "/mfcc");
//   m.addFloatArg(mfcc);
//   return m;
// }

// ofxOscMessage Channel::getMessageHpcp (int numChannel) {
//   // vector<float> hpcp;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel) + "/hpcp");
//   m.addFloatArg(hpcp);
//   return m;
// }

// ofxOscMessage Channel::getMessageTristimulus (int numChannel) {
//   // vector<float> tristimulus;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel) + "/tristimulus");
//   m.addFloatArg(tristimulus);
//   return m;
// }

ofxOscMessage Channel::getMessageIsOnset (int numChannel) {
  // bool isOnset;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel) + "/isOnset");
  m.addBoolArg(isOnset);
  return m;
}

