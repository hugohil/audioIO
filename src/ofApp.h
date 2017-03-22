#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"
#include "ofxDatGui.h"
#include "ofxSocketIO.h"
#include "ofxSocketIOData.h"
#include "gui/theme.h"

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

    void onButtonEvent(ofxDatGuiButtonEvent e);

    void connect();
    ofxDatGuiButton* connectButton;
    ofxDatGuiTextInput* hostInput;
    ofxDatGuiTextInput* portInput;
    string host;
    string port;

    void onDevicesDropdownEvent(ofxDatGuiDropdownEvent e);

    vector<ofSoundDevice> deviceList;
    ofSoundStream soundStream;
    ofxAudioAnalyzer audioAnalyzer;

    int sampleRate;
    int bufferSize;
    int outChannels;
    int inChannels;

    float RMSThreshold = 0.1;
    float onSetsAlpha = 1.0;
    float onSetsSilenceThreshold = 0.2;
    float onSetsUseTimeThreshold = true;
    float onSetsTimeThreshold = 100.0;
    float smoothing = 0.75;

    ofxDatGui* networkGUI;
    ofxDatGui* deviceGUI;
    ofxDatGui* audioGUI;

    ofxSocketIO socketIO;
    void onConnection();
};
