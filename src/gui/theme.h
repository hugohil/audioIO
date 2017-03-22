#pragma once
#include "ofxDatGuiTheme.h"

class ofxDatGuiThemeFUBAR : public ofxDatGuiTheme{

public:

    ofxDatGuiThemeFUBAR()
    {
        icon.radioOnPath = "gui/images/icon-radio-on.png";
        icon.radioOffPath = "gui/images/icon-radio-off.png";
        icon.groupOpenPath = "gui/images/icon-group-open.png";
        icon.groupClosedPath = "gui/images/icon-group-closed.png";
        icon.rainbowPath = "gui/images/picker-rainbow.png";
        font.file = "gui/fonts/Verdana.ttf";

        init();
    }
};
