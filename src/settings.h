//
//  settings.hpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 27/07/17.
//
//

#include "ofMain.h"

class settings {

    public:
        void setup();
    
        void updateVideoSettings(vector<ofPoint> newVideoPosition);
        void saveVideoSettings();
    
        // General Settings
        ofXml touchWallSettings;
        // TouchBoard ID
        int touchDeviceId;
        // Video Type
        string videoType;
        // Video Loading Mode
        string videoLoadingMode;
        // Video Link
        string videoLink;
        // GUI
        bool guiActive;
    
        // Video Settings Xml
        ofXml videoSettings;
        // Video Filename
        vector <string> videoName;
        // Video Description
        vector <string> videoDesc;
        // Video Width
        vector <float> videoWidth;
        // Video Position in Screen
        vector <ofPoint> videoPosition;
        // Video Associated Electrode
        vector <string> videoElectrode;

};
