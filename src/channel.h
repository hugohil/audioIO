#pragma once

#include "ofxOsc.h"

class Channel {
  public:
    ofxOscMessage getMessageRms();
    ofxOscMessage getMessagePower();
    ofxOscMessage getMessagePitchFreq();
    ofxOscMessage getMessagePitchFreqNorm();
    ofxOscMessage getMessagePitchConf();
    ofxOscMessage getMessagePitchSalience();
    ofxOscMessage getMessageHfc();
    ofxOscMessage getMessageHfcNorm();
    ofxOscMessage getMessageSpecComp();
    ofxOscMessage getMessageSpecCompNorm();
    ofxOscMessage getMessageCentroid();
    ofxOscMessage getMessageCentroidNorm();
    ofxOscMessage getMessageInharmonicity();
    ofxOscMessage getMessageDissonance();
    ofxOscMessage getMessageRollOff();
    ofxOscMessage getMessageRollOffNorm();
    ofxOscMessage getMessageOddToEven();
    ofxOscMessage getMessageOddToEvenNorm();
    ofxOscMessage getMessageStrongPeak();
    ofxOscMessage getMessageStrongPeakNorm();
    ofxOscMessage getMessageStrongDecay();
    ofxOscMessage getMessageStrongDecayNorm();
    // ofxOscMessage getMessageSpectrum();
    // ofxOscMessage getMessageMelBands();
    // ofxOscMessage getMessageMfcc();
    // ofxOscMessage getMessageHpcp();
    // ofxOscMessage getMessageTristimulus();
    ofxOscMessage getMessageIsOnset();

    int numChannel;

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