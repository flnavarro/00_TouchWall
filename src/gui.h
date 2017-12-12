//
//  gui.hpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 31/07/17.
//
//
#pragma once

#include "ofMain.h"

class gui {
    
    public:
        void setup(int numElect, bool guiActive);
        void update(vector<bool> touchStatus);
        void draw(int alpha);
    
        void drawTouchBdStatus();
        void changeStatus();
    
        // Number of Active Electrodes
        int nElect;
    
        // Touch Board Status
        vector<bool> touchBdStatus;
    
        // GUI
        bool isActive;
    
        // Image
        ofImage panel;
};