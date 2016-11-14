#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{
  public:
    void setup();
    void update();
    void draw();
    void exit();

    void audioIn(ofSoundBuffer &inBuffer);

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void onDevicesDropdownEvent(ofxDatGuiDropdownEvent e);

    vector<ofSoundDevice> deviceList;
    ofSoundStream soundStream;
    ofxAudioAnalyzer audioAnalyzer;

    int sampleRate;
    int bufferSize;
    int outChannels;
    int inChannels;

    vector<float> RMSs;

    ofxDatGui* audioSetupGUI;
};
