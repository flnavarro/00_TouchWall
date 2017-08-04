//
//  settings.cpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 27/07/17.
//
//

#include "settings.h"

void settings::setup(){
    
    // General Settings By Default
    // Device ID for TouchBoard
    touchDeviceId = 1;
    // Video Type (VID o PNG)
    videoType = "VID";
    // Video Loading Mode (INIT o REQUEST)
    videoLoadingMode = "INIT";
    // Forma de Asociar Videos (AUTO o XML) - **INACTIVO DE MOMENTO**
    videoLink = "AUTO";
    // Active GUI
    guiActive = true;
    
    // NO LEEMOS ESTO DE MOMENTO
    /*
    // Load xml with general settings
    if(touchWallSettings.load("xml/touchWallSettings.xml")){
        if(touchWallSettings.exists("//touchDeviceId")){
            touchDeviceId = touchWallSettings.getValue<int>("//touchDeviceId");
        }
        if(touchWallSettings.exists("//videoType")){
            videoType = touchWallSettings.getValue<string>("//videoType");
        }
        if(touchWallSettings.exists("//videoLoadingMode")){
            videoLoadingMode = touchWallSettings.getValue<string>("//videoLoadingMode");
        }
        if(touchWallSettings.exists("//videoLink")){
            videoLink = touchWallSettings.getValue<string>("//videoLink");
        }
        if(touchWallSettings.exists("//guiActive")){
            if(touchWallSettings.getValue<string>("//guiActive") == "YES"){
                guiActive = true;
            } else if (touchWallSettings.getValue<string>("//guiActive") == "NO"){
                guiActive = false;
            }
        }
    }
    */
    
    // Load xml with video positions
    if(videoSettings.load("xml/videoSettings.xml")){
        if(videoSettings.exists("//video")){
            videoSettings.setTo("//video[0]");
            do {
                videoName.push_back(videoSettings.getValue<string>("name"));
                videoDesc.push_back(videoSettings.getValue<string>("desc"));
                videoWidth.push_back(videoSettings.getValue<float>("videoWidth"));
                ofPoint p;
                p.x = videoSettings.getValue<float>("posX");
                p.y = videoSettings.getValue<float>("posY");
                videoPosition.push_back(p);
                videoElectrode.push_back(videoSettings.getValue<string>("electrode"));
            }
            while(videoSettings.setToSibling());
        }
    }else{
        ofLogError("Unable to load videoPositions.xml, check data folder");
    }
}

void settings::updateVideoSettings(vector<ofPoint> newVideoPosition){
    // Update Video Position
    videoPosition = newVideoPosition;
    // Save New Positions
    saveVideoSettings();
}

void settings::saveVideoSettings(){
    // Save New Positions to Xml
    if(videoSettings.load("xml/videoSettings.xml")){
        if(videoSettings.exists("//video")){
            for(int i=0; i<videoPosition.size(); i++){
                string pathX = "//video[" + ofToString(i) + "]/posX";
                string pathY = "//video[" + ofToString(i) + "]/posY";
                videoSettings.setValue(pathX, ofToString(videoPosition[i].x));
                videoSettings.setValue(pathY, ofToString(videoPosition[i].y));
            }
            videoSettings.save("xml/videoSettings.xml");
        }
    }
}