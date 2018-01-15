#pragma once

#include "ofMain.h"
#include "ofxAudioAnalyzer.h"
#include "ofxDatGui.h"
#include "ofxSocketIO.h"
#include "ofxSocketIOData.h"
#include "gui/theme.h"

class ofApp : public ofBaseApp{
  public:
    vector<string> arguments;

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
    ofxDatGuiLabel* connectionLabel;
    ofxDatGuiTextInput* hostInput;
    ofxDatGuiTextInput* portInput;
    string host = "127.0.0.1";
    string port = "8888";

    void onDevicesDropdownEvent(ofxDatGuiDropdownEvent e);
    void onSampleRateDropdownEvent(ofxDatGuiDropdownEvent e);
    void onBufferSizeDropdownEvent(ofxDatGuiDropdownEvent e);
    void onActiveChannelsSliderEvent(ofxDatGuiSliderEvent e);

    int deviceIndex = 0;
    vector<ofSoundDevice> deviceList;
    ofSoundStream soundStream;
    ofxAudioAnalyzer audioAnalyzer;
    ofSoundDevice device;
    ofxDatGuiButton* startButton;
    bool isStreamActive = false;
    ofxDatGuiLabel* streamLabel;
    void toggleStream();
    void setupDevice();

    vector<string> sampleRates = { "8000", "16000", "32000", "44100", "48000", "96000" };
    vector<string> bufferSizes = { "2048", "1024", "512", "256", "128", "64", "32", "16" };
    int sampleRate = 44100;
    int bufferSize = 256;
    int outChannels;
    int inChannels = 2;
    int activeChannels = 1;

    float RMSThreshold = 0.1;
    float onSetsAlpha = 1.0;
    float onSetsSilenceThreshold = 0.2;
    float onSetsUseTimeThreshold = true;
    float onSetsTimeThreshold = 100.0;
    float smoothing = 0.75;

    ofxDatGui* networkGUI;
    ofxDatGui* deviceGUI;
    ofxDatGui* audioGUI;

    bool autostart = false;

    ofxSocketIO socketIO;
    void onConnection();
};
