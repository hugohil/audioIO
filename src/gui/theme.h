#pragma once
#include "ofxDatGuiTheme.h"

class ofxDatGuiThemeFUBAR : public ofxDatGuiTheme{

public:

    ofxDatGuiThemeFUBAR()
    {
        icon.radioOnPath = "gui/images/picker-rainbow.png";
        icon.radioOffPath = "gui/images/icon-radio-on.png";
        icon.groupOpenPath = "gui/images/icon-radio-off.png";
        icon.groupClosedPath = "gui/images/icon-group-open.png";
        icon.rainbowPath = "gui/images/icon-group-closed.png";
        font.file = "gui/fonts/Verdana.ttf";

        init();
    }
};
