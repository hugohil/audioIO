#!/bin/bash

if [ -z $1 ]; then
PREFIX="git clone https://github.com/"
else
PREFIX="git clone git@github.com:"
fi

cd ../../../addons

#ofxAudioAnalyzer------------------------
${PREFIX}hugohil/ofxAudioAnalyzer.git

#ofxAudioDecoder------------------------
${PREFIX}leozimmerman/ofxAudioDecoder.git

#ofxDatGui------------------------
${PREFIX}hugohil/ofxDatGui.git
cd ofxDatGui
git checkout update-anchors
cd ../

#ofxSocketIO------------------------
${PREFIX}soixantecircuits/ofxSocketIO.git
