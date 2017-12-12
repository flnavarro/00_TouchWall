//
//  touchBoard.cpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 27/07/17.
//
//

#include "touchBoard.h"

void touchBoard::setup(int nElect, int touchDeviceId){
    
    // Number of Active Electrodes
    nElectrodes = nElect;

    // Touch Board Setup
    touchBd.setup(touchDeviceId);
    ofAddListener(touchBd.touched, this, &touchBoard::touched);
    ofAddListener(touchBd.released, this, &touchBoard::released);
    
    // Initialize Touch Status
    for(int i=0; i<nElectrodes; i++){
        touchStatus.push_back(false);
    }
}

void touchBoard::update(){
    // Update Touch Status
    touchBd.update();
    for(int i=0; i<nElectrodes; i++){
        touchStatus[i] = touchBd.getTouchStatus()[i];
    }
}

void touchBoard::touched(int& id){
    ofLog() << "Electrode " << id << " touched";
}

void touchBoard::released(int& id){
    ofLog() << "Electrode " << id << " released";
}
