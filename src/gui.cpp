//
//  gui.cpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 31/07/17.
//
//

#include "gui.h"


void gui::setup(int numElect,  bool guiActive){
    
    nElect = numElect;
    
    isActive = guiActive;
    
    panel.load("debugImg/panelFinal.png");
}

void gui::update(vector<bool> touchStatus){
    touchBdStatus = touchStatus;
}

void gui::draw(int alpha){
    if(isActive){
        ofPushStyle();
        ofSetColor(255,255,255,alpha);
        panel.draw(0, 0);
        drawTouchBdStatus();
        ofPopStyle();
    }
}

void gui::drawTouchBdStatus(){
    float radius = 10.0;
    float space = 20.0;
    for(int i = 0; i < nElect; i++){
        if(touchBdStatus[i]){
            ofSetHexColor(0x82D87F);
        } else {
            ofSetHexColor(0x959595);
        }
        ofDrawCircle(space + (2*radius + space) * i + radius, 20.0, radius);
    }
}

void gui::changeStatus(){
    isActive = !isActive;
    if(isActive){
        ofShowCursor();
    } else {
        ofHideCursor();
    }
}