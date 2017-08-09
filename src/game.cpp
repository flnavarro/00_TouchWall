//
//  game.cpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 04/08/17.
//
//

#include "game.h"

void game::setup(int nElect){
    
    // Bare Conductive
    nElectrodes = nElect;
    
    // Get Images
    string dirPath;
    string fileName;
    for( int j=0; j<3; j++ ){
        // Get PNG Folder Directory
        dir.listDir("gamePNG/Postazione" + ofToString(j+1));
        int questionId = 0;
        int imgId = 0;
        for(int i=0; i<dir.size(); i++){
            // Cargar imagen
            dirPath = dir.getPath(i);
            fileName = dir.getName(i);
            postazione[j][questionId][imgId].load(dirPath);
            int width = postazione[j][questionId][imgId].getWidth();
            int height = postazione[j][questionId][imgId].getHeight();
            postazione[j][questionId][imgId].resize((int)((float)width*0.35),
                                                    (int)((float)height*0.35));
            postImgName[j][questionId][imgId] = fileName;
            // Aumentar index
            if( imgId < 5-1 ){
                imgId++;
            } else {
                questionId++;
                imgId=0;
            }
        }
    }
    
    // Inicializar touch status
    for(int i=0; i<nElectrodes; i++){
        touchStatus.push_back(false);
    }
    
    loadButtons();
    
    // TODO: CHANGE THIS
    alphaPostazione[0] = 255;
    alphaPostazione[1] = 255;
    alphaPostazione[2] = 255;
    
    postazionePos[0] = ofVec2f(0, 100);
    postazionePos[1] = ofVec2f(600, 100);
    postazionePos[2] = ofVec2f(1200, 100);
    
    timeFont.load("fonts/Verdana.ttf", 10);
    pointsFont.load("fonts/Verdana.ttf", 60);
}

void game::update(vector<bool> touchStatus){
    // Recorremos las 3 postaziones
    for( int i=0; i<3; i++ ){
        // Si se toca el botón touch de Postazione y el juego no está activo
        if(!postazioneStatus[i] && touchStatus[postElectIndex[i]]){
            // Comenzar juego
            startPostazione(i);
        }
        // Si el juego está activo
        if(postazioneStatus[i]){
            ofLog() << "POSTAZIONE " << i << " // STEP-> " << postazioneStep[i];
            if(postazioneStep[i] == "pre-game"){
                
            }
            // Si estamos esperando a la respuesta del usuario
            else if(postazioneStep[i] == "waiting for answer"){
                if(ofGetElapsedTimef()-lastElapsedTime[i] < maxAnswerTime){
                    // Recorremos los 3 botones ABC
                    for( int j=0; j<3; j++ ){
                        // Si se toca algún botón ABC
                        if(touchStatus[optionElectIndex[i][j]]){
                            // Cargar imagen correspondiente de respuesta
                            int newImgIndex = j+1;
                            imgId[i] = newImgIndex;
                            // Comprobar si la respuesta es correcta para sumar puntuación
                            if(newImgIndex == postCorrectAnswer[i][questionId[i]]){
                                postazionePoints[i] += 20;
                            }
                            // Cambiar a paso de enseñar la respuesta
                            postazioneStep[i] = "showing answer";
                            updateTimer(i);
                        }
                    }
                } else {
                    imgId[i] = 4;
                    postazioneStep[i] = "showing timeout";
                    updateTimer(i);
                }
            }
            else if(postazioneStep[i] == "showing answer"
                    || postazioneStep[i] == "showing timeout"){
                if(ofGetElapsedTimef()-lastElapsedTime[i] > timeToNextQuestion){
                    if(questionId[i] + 1 < 5){
                        questionId[i]++;
                        imgId[i] = 0;
                        postazioneStep[i] = "waiting for answer";
                    } else if (questionId[i] + 1 == 5) {
                        questionId[i]++;
                        if(postazionePoints[i] >= 60){
                            imgId[i] = 1;
                        } else {
                            imgId[i] = 0;
                        }
                        postazioneStep[i] = "showing points";
                    }
                    updateTimer(i);
                }
            }
            else if(postazioneStep[i] == "showing points"){
                if(ofGetElapsedTimef()-lastElapsedTime[i] > timeToEnjoyPoints){
                    postazioneStep[i] = "pre-game";
                    postazioneStatus[i] = false;
                }
            }
        }
    }
}

void game::draw(){
    for( int i=0; i<3; i++ ){
        if(postazioneStatus[i]){
            ofSetColor(255, 255, 255, alphaPostazione[i]);
            postazione[i][questionId[i]][imgId[i]].draw(postazionePos[i]);
            if (postazioneStep[i] == "waiting for answer"){
                timeFont.drawString(ofToString(maxAnswerTime-(ofGetElapsedTimef()-lastElapsedTime[i])), postazionePos[i].x+350, postazionePos[i].y+100);
            } else if(postazioneStep[i] == "showing points"){
                pointsFont.drawString(ofToString(postazionePoints[i]),
                                      postazionePos[i].x+350, postazionePos[i].y+400);
            }
        }
    }
}

void game::startPostazione(int postId){
    
    questionId[postId] = 0;
    imgId[postId] = 0;
    
    // "pre-game"
    // "waiting for answer"
    // "showing answer" or "showing timeout"
    // "showing points"
    postazioneStep[postId] = "waiting for answer";
    updateTimer(postId);
    
    postazionePoints[postId] = 0;
    
    postazioneStatus[postId] = true;
}

void game::updateTimer(int postId){
    lastElapsedTime[postId] = ofGetElapsedTimef();
}

void game::loadButtons(){
    // Botones Postazione -> Index Electrodo
    // Postazione 1
    postElectIndex[0] = 0;
    // Postazione 2
    postElectIndex[1] = 1;
    // Postazione 3
    postElectIndex[2] = 2;
    
    // Botones Opciones -> Index Electrodo
    // Postazione 1 - Botón A
    optionElectIndex[0][0] = 3;
    // Postazione 1 - Botón B
    optionElectIndex[0][1] = 4;
    // Postazione 1 - Botón C
    optionElectIndex[0][2] = 5;
    // Postazione 2 - Botón A
    optionElectIndex[1][0] = 6;
    // Postazione 2 - Botón B
    optionElectIndex[1][1] = 7;
    // Postazione 2 - Botón C
    optionElectIndex[1][2] = 8;
    // Postazione 3 - Botón A
    optionElectIndex[2][0] = 9;
    // Postazione 3 - Botón B
    optionElectIndex[2][1] = 10;
    // Postazione 3 - Botón C
    optionElectIndex[2][2] = 11;
    
    postCorrectAnswer[0][0] = 1;
    postCorrectAnswer[0][1] = 2;
    postCorrectAnswer[0][2] = 3;
    postCorrectAnswer[0][3] = 1;
    postCorrectAnswer[0][4] = 2;
    postCorrectAnswer[1][0] = 1;
    postCorrectAnswer[1][1] = 2;
    postCorrectAnswer[1][2] = 3;
    postCorrectAnswer[1][3] = 1;
    postCorrectAnswer[1][4] = 2;
    postCorrectAnswer[2][0] = 1;
    postCorrectAnswer[2][1] = 2;
    postCorrectAnswer[2][2] = 3;
    postCorrectAnswer[2][3] = 1;
    postCorrectAnswer[2][4] = 2;
}