#pragma once

#include "ofxOsc.h"

class Channel {
  public:
    ofxOscMessage getMessageRms(int numChannel);
    ofxOscMessage getMessagePower(int numChannel);
    ofxOscMessage getMessagePitchFreq(int numChannel);
    ofxOscMessage getMessagePitchFreqNorm(int numChannel);
    ofxOscMessage getMessagePitchConf(int numChannel);
    ofxOscMessage getMessagePitchSalience(int numChannel);
    ofxOscMessage getMessageHfc(int numChannel);
    ofxOscMessage getMessageHfcNorm(int numChannel);
    ofxOscMessage getMessageSpecComp(int numChannel);
    ofxOscMessage getMessageSpecCompNorm(int numChannel);
    ofxOscMessage getMessageCentroid(int numChannel);
    ofxOscMessage getMessageCentroidNorm(int numChannel);
    ofxOscMessage getMessageInharmonicity(int numChannel);
    ofxOscMessage getMessageDissonance(int numChannel);
    ofxOscMessage getMessageRollOff(int numChannel);
    ofxOscMessage getMessageRollOffNorm(int numChannel);
    ofxOscMessage getMessageOddToEven(int numChannel);
    ofxOscMessage getMessageOddToEvenNorm(int numChannel);
    ofxOscMessage getMessageStrongPeak(int numChannel);
    ofxOscMessage getMessageStrongPeakNorm(int numChannel);
    ofxOscMessage getMessageStrongDecay(int numChannel);
    ofxOscMessage getMessageStrongDecayNorm(int numChannel);
    // ofxOscMessage getMessageSpectrum(int numChannel);
    // ofxOscMessage getMessageMelBands(int numChannel);
    // ofxOscMessage getMessageMfcc(int numChannel);
    // ofxOscMessage getMessageHpcp(int numChannel);
    // ofxOscMessage getMessageTristimulus(int numChannel);
    ofxOscMessage getMessageIsOnset(int numChannel);

    float rms;
    float power;
    float pitchFreq;
    float pitchFreqNorm;
    float pitchConf;
    float pitchSalience;
    float hfc;
    float hfcNorm;
    float specComp;
    float specCompNorm;
    float centroid;
    float centroidNorm;
    float inharmonicity;
    float dissonance;
    float rollOff;
    float rollOffNorm;
    float oddToEven;
    float oddToEvenNorm;
    float strongPeak;
    float strongPeakNorm;
    float strongDecay;
    float strongDecayNorm;
    vector<float> spectrum;
    vector<float> melBands;
    vector<float> mfcc;
    vector<float> hpcp;
    vector<float> tristimulus;
    bool isOnset;
};