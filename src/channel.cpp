#include "channel.h"

ofxOscMessage Channel::getMessageRms () {
  // float rms;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/rms");
  m.addFloatArg(rms);
  return m;
}

ofxOscMessage Channel::getMessagePower () {
  // float power;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/power");
  m.addFloatArg(power);
  return m;
}

ofxOscMessage Channel::getMessagePitchFreq () {
  // float pitchFreq;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/pitchFreq");
  m.addFloatArg(pitchFreq);
  return m;
}

ofxOscMessage Channel::getMessagePitchFreqNorm () {
  // float pitchFreqNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/pitchFreqNorm");
  m.addFloatArg(pitchFreqNorm);
  return m;
}

ofxOscMessage Channel::getMessagePitchConf () {
  // float pitchConf;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/pitchConf");
  m.addFloatArg(pitchConf);
  return m;
}

ofxOscMessage Channel::getMessagePitchSalience () {
  // float pitchSalience;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/pitchSalience");
  m.addFloatArg(pitchSalience);
  return m;
}

ofxOscMessage Channel::getMessageHfc () {
  // float hfc;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/hfc");
  m.addFloatArg(hfc);
  return m;
}

ofxOscMessage Channel::getMessageHfcNorm () {
  // float hfcNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/hfcNorm");
  m.addFloatArg(hfcNorm);
  return m;
}

ofxOscMessage Channel::getMessageSpecComp () {
  // float specComp;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/specComp");
  m.addFloatArg(specComp);
  return m;
}

ofxOscMessage Channel::getMessageSpecCompNorm () {
  // float specCompNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/specCompNorm");
  m.addFloatArg(specCompNorm);
  return m;
}

ofxOscMessage Channel::getMessageCentroid () {
  // float centroid;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/centroid");
  m.addFloatArg(centroid);
  return m;
}

ofxOscMessage Channel::getMessageCentroidNorm () {
  // float centroidNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/centroidNorm");
  m.addFloatArg(centroidNorm);
  return m;
}

ofxOscMessage Channel::getMessageInharmonicity () {
  // float inharmonicity;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/inharmonicity");
  m.addFloatArg(inharmonicity);
  return m;
}

ofxOscMessage Channel::getMessageDissonance () {
  // float dissonance;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/dissonance");
  m.addFloatArg(dissonance);
  return m;
}

ofxOscMessage Channel::getMessageRollOff () {
  // float rollOff;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/rollOff");
  m.addFloatArg(rollOff);
  return m;
}

ofxOscMessage Channel::getMessageRollOffNorm () {
  // float rollOffNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/rollOffNorm");
  m.addFloatArg(rollOffNorm);
  return m;
}

ofxOscMessage Channel::getMessageOddToEven () {
  // float oddToEven;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/oddToEven");
  m.addFloatArg(oddToEven);
  return m;
}

ofxOscMessage Channel::getMessageOddToEvenNorm () {
  // float oddToEvenNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/oddToEvenNorm");
  m.addFloatArg(oddToEvenNorm);
  return m;
}

ofxOscMessage Channel::getMessageStrongPeak () {
  // float strongPeak;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/strongPeak");
  m.addFloatArg(strongPeak);
  return m;
}

ofxOscMessage Channel::getMessageStrongPeakNorm () {
  // float strongPeakNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/strongPeakNorm");
  m.addFloatArg(strongPeakNorm);
  return m;
}

ofxOscMessage Channel::getMessageStrongDecay () {
  // float strongDecay;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/strongDecay");
  m.addFloatArg(strongDecay);
  return m;
}

ofxOscMessage Channel::getMessageStrongDecayNorm () {
  // float strongDecayNorm;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/strongDecayNorm");
  m.addFloatArg(strongDecayNorm);
  return m;
}

// ofxOscMessage Channel::getMessageSpectrum () {
//   // vector<float> spectrum;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/spectrum");
//   m.addFloatArg(spectrum);
//   return m;
// }

// ofxOscMessage Channel::getMessageMelBands () {
//   // vector<float> melBands;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/melBands");
//   m.addFloatArg(melBands);
//   return m;
// }

// ofxOscMessage Channel::getMessageMfcc () {
//   // vector<float> mfcc;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/mfcc");
//   m.addFloatArg(mfcc);
//   return m;
// }

// ofxOscMessage Channel::getMessageHpcp () {
//   // vector<float> hpcp;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/hpcp");
//   m.addFloatArg(hpcp);
//   return m;
// }

// ofxOscMessage Channel::getMessageTristimulus () {
//   // vector<float> tristimulus;
//   ofxOscMessage m;
//   m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/tristimulus");
//   m.addFloatArg(tristimulus);
//   return m;
// }

ofxOscMessage Channel::getMessageIsOnset () {
  // bool isOnset;
  ofxOscMessage m;
  m.setAddress("/audioIO/" + ofToString(numChannel + 1.0) + "/isOnset");
  m.addBoolArg(isOnset);
  return m;
}

