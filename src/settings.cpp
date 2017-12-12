//
//  settings.cpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 27/07/17.
//
//

#include "settings.h"

void settings::setup(){
    
    // Load xml with general settings
    if(touchWallSettings.load("xmlSettings/touchWallSettings.xml")){
        if(touchWallSettings.exists("//touchDeviceId")){
            touchDeviceId = touchWallSettings.getValue<int>("//touchDeviceId");
        }
        string xmlField;
        for(int i=0; i<3; i++){
            xmlField = "//postazione" + ofToString(i+1) + "_Play";
            if(touchWallSettings.exists(xmlField)){
                postazioneElectIndex[i] = touchWallSettings.getValue<int>(xmlField);
            }
            xmlField = "//postazione" + ofToString(i+1) + "_A";
            if(touchWallSettings.exists(xmlField)){
                abcElectIndex[i][0] = touchWallSettings.getValue<int>(xmlField);
            }
            xmlField = "//postazione" + ofToString(i+1) + "_B";
            if(touchWallSettings.exists(xmlField)){
                abcElectIndex[i][1] = touchWallSettings.getValue<int>(xmlField);
            }
            xmlField = "//postazione" + ofToString(i+1) + "_C";
            if(touchWallSettings.exists(xmlField)){
                abcElectIndex[i][2] = touchWallSettings.getValue<int>(xmlField);
            }
        }
    }
    
    // Load xml with video positions
    if(gameSettings.load("xmlSettings/gameSettings.xml")){
        if(gameSettings.exists("//guiActive")){
            if(gameSettings.getValue<string>("//guiActive") == "YES"){
                guiActive = true;
            } else if (gameSettings.getValue<string>("//guiActive") == "NO"){
                guiActive = false;
            }
        }
        if(gameSettings.exists("//showRemainingTime")){
            if(gameSettings.getValue<string>("//showRemainingTime") == "YES"){
                showRemainingTime = true;
            } else if (gameSettings.getValue<string>("//showRemainingTime") == "NO"){
                showRemainingTime = false;
            }
        }
        if(gameSettings.exists("//sequenceFPS")){
            sequenceFPS = gameSettings.getValue<int>("//sequenceFPS");
        }
        if(gameSettings.exists("//maxAnswerTime")){
            maxAnswerTime = gameSettings.getValue<float>("//maxAnswerTime");
        }
        if(gameSettings.exists("//lastSecondsTime")){
            lastSecondsTime = gameSettings.getValue<float>("//lastSecondsTime");
        }
        if(gameSettings.exists("//toNextQuestionTime")){
            toNextQuestionTime = gameSettings.getValue<float>("//toNextQuestionTime");
        }
        if(gameSettings.exists("//pointsTime")){
            pointsTime = gameSettings.getValue<float>("//pointsTime");
        }
        if(gameSettings.exists("//pointsToPass")){
            pointsToPass = gameSettings.getValue<int>("//pointsToPass");
        }
        
        string xmlField;
        for(int i=0; i<3; i++){
            for(int j=0; j<5; j++){
                xmlField = "//postazione" + ofToString(i+1) + "/question" + ofToString(j+1) + "/correctAnswer";
                if(gameSettings.exists(xmlField)){
                    if(gameSettings.getValue<string>(xmlField) == "A"){
                        postCorrectAnswer[i][j] = 0;
                    } else if(gameSettings.getValue<string>(xmlField) == "B"){
                        postCorrectAnswer[i][j] = 1;
                    } else if(gameSettings.getValue<string>(xmlField) == "C"){
                        postCorrectAnswer[i][j] = 2;
                    }
                }
                xmlField = "//postazione" + ofToString(i+1) + "/question" + ofToString(j+1) + "/numPossibleAnswers";
                if(gameSettings.exists(xmlField)){
                    numAnswerPerQuestion[i][j] = gameSettings.getValue<int>(xmlField);
                }
            }
        }
    }else{
        ofLogError("Unable to load xml settings file, check data folder");
    }
}