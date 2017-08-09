//
//  gui.cpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 31/07/17.
//
//

#include "gui.h"


void gui::setup(int numElect,  bool guiActive, vector<ofPoint> videoPosition){
    
    nElect = numElect;
    
    isActive = guiActive;
    
//    for(int i=0; i<nElect; i++){
//        videoGui.push_back(new ofxDatGui( ofxDatGuiAnchor::TOP_LEFT ));
//        videoGui[i]->addHeader("Vid E" + ofToString(i));
//        videoGui[i]->setPosition(videoPosition[i].x, videoPosition[i].y);
//        guiPos.push_back(videoGui[i]->getPosition());
//    }
    
    panel.load("debugImg/panel.png");
}

void gui::update(vector<bool> touchStatus){
    touchBdStatus = touchStatus;
    
//    if(isActive){
//        updateVideoSettings = false;
//        
//        for(int i=0; i<nElect; i++){
//            videoGui[i]->update();
//            ofPoint p;
//            p = videoGui[i]->getPosition();
//            if(p!=guiPos[i]){
//                guiPos[i] = p;
//                updateVideoSettings = true;
//            }
//        }
//    }
}

void gui::draw(){
    if(isActive){
        float index = ofGetWidth()/panel.getWidth();
        panel.draw(0, 0, ofGetWidth(), panel.getHeight()*index);
        drawTouchBdStatus();
    }
}

void gui::drawTouchBdStatus(){
    float radius = 10.0;
    float space = 20.0;
    // float space = (ofGetWidth()/2 - 2 * nElect * radius) / (nElect + 1);
    
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
//    for(int i=0; i<nElect; i++){
//        videoGui[i]->setVisible(isActive);
//    }
}