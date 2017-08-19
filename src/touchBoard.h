//
//  touchBoard.hpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 27/07/17.
//
//
#pragma once

#include "ofMain.h"

#include "ofxTouchBoard.h"

class touchBoard {
    
    public:
        void setup(int nElect, int touchDeviceId);
        void update();
    
        void touched(int& e);
        void released(int& e);
    
        // Bare Conductive
        ofxTouchBoard touchBd;
        int nElectrodes;
        vector<bool> touchStatus;
};
