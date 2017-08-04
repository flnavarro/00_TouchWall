//
//  gui.hpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 31/07/17.
//
//

#include "ofMain.h"

#include "ofxDatGui.h"

class gui {
    
    public:
        void setup(int numElect, bool guiActive,
                   vector<ofPoint> videoPosition);
        void update(vector<bool> touchStatus);
        void draw();
    
        void drawTouchBdStatus();
        void changeStatus();
    
        // Number of Active Electrodes
        int nElect;
    
        // Touch Board Status
        vector<bool> touchBdStatus;
    
        // GUI
        bool isActive;
        vector<ofxDatGui*> videoGui;
        vector<ofPoint> guiPos;
        bool updateVideoSettings;
    
};