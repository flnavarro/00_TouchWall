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
    
    // Inicializar touch status
    for(int i=0; i<nElectrodes; i++){
        touchStatus.push_back(false);
    }
    
    loadButtons();
    
    loadAllImages();
    
    postazione_0 = ofVec2f(7, 113);
    postazionePos[0] = ofVec2f(305, 55);
    postazionePos[1] = ofVec2f(750, -25);
    postazionePos[2] = ofVec2f(1350, 55);
    
    // ** USEFUL TO DEBUG ** //
    is_debugging = false;
    
    postazioneStatus[0] = false;
    postazioneStatus[1] = postazioneStatus[0];
    postazioneStatus[2] = postazioneStatus[0];
    
    string init_step[6];
    init_step[0] = "waiting touch";
    init_step[1] = "pre-game";
    init_step[2] = "waiting for answer";
    init_step[3] = "showing answer";
    init_step[4] = "showing timeout";
    init_step[5] = "showing points";
    postazioneStep[0] = init_step[0];
    postazioneStep[1] = postazioneStep[0];
    postazioneStep[2] = postazioneStep[0];
    // ************* //
    
    touchwallStatus = false;
    
    sequenceFPS = 2;
    
    // TODO: CHANGE THIS
    alphaPostazione[0] = 255;
    alphaPostazione[1] = 255;
    alphaPostazione[2] = 255;
    
    timeFont.load("fonts/Verdana.ttf", 10);
    pointsFont.load("fonts/Verdana.ttf", 60);
}

void game::update(vector<bool> touchStatus){
    if(!postazioneStatus[0] && !postazioneStatus[1] && !postazioneStatus[2]){
        touchwallStatus = false;
    } else {
        touchwallStatus = true;
    }
    
    if( !touchwallStatus ||
        (postazioneStep[0]!="pre-game" &&
         postazioneStep[1]!="pre-game" &&
         postazioneStep[2]!="pre-game")
       ){
        frameIndex_p0 = (int)(ofGetElapsedTimef() * sequenceFPS) % a_pos0_st0.size();
    }
    
    // Recorremos las 3 postaziones
    for( int i=0; i<3; i++ ){
        // Si se toca el botón touch de Postazione y el juego no está activo
        if(!postazioneStatus[i] && touchStatus[postElectIndex[i]] && !is_debugging){
            // Comenzar juego
            startPostazione(i);
        }
        if(postazioneStatus[i]){
            ofLog() << "POSTAZIONE " << i << " // STEP-> " << postazioneStep[i];
            float thisElapsedTime = ofGetElapsedTimef() - lastElapsedTime[i];
            if(postazioneStep[i] == "pre-game"){
                frameIndex_p123[i] = (int)(thisElapsedTime * sequenceFPS) % a_pos123_st2[i].size();
                if(frameIndex_p123[i] == 0 && prevFrameIndex_p123[i]!=0 && !is_debugging){
                    postazioneStep[i] = "waiting for answer";
                    frameIndex_p123[i] = 0;
                    prevFrameIndex_p123[i] = 0;
                }
                prevFrameIndex_p123[i] = frameIndex_p123[i];
            } else if(postazioneStep[i] == "waiting for answer"){
                if(thisElapsedTime < maxAnswerTime){
                    // Recorremos los 3 botones ABC
                    for(int j=0; j<3; j++){
                        // Si se toca algún botón ABC
                        if(touchStatus[optionElectIndex[i][j]]){
                            // Cargar imagen correspondiente de respuesta
                            imgAnswerId[i] = j;
                            // Comprobar si la respuesta es correcta para sumar puntuación
                            if(imgAnswerId[i] == postCorrectAnswer[i][questionId[i]]){
                                postazionePoints[i] += 20;
                            }
                            // Cambiar a paso de enseñar la respuesta
                            postazioneStep[i] = "showing answer";
                            updateTimer(i);
                        }
                        if(thisElapsedTime < maxAnswerTime-lastSecondsAmount){
                            frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_wait[i].size();
                            kid_wait_st3[i] = true;
                        } else {
                            frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_time[i].size();
                            kid_wait_st3[i] = false;
                        }
                    }
                } else {
                    // Si se acaba el tiempo - enseñar timeout
                    imgAnswerId[i] = 3;
                    postazioneStep[i] = "showing timeout";
                    updateTimer(i);
                }
            } else if(postazioneStep[i] == "showing answer" ||
                      postazioneStep[i] == "showing timeout"){
                if(ofGetElapsedTimef()-lastElapsedTime[i] > timeToNextQuestion){
                    if(questionId[i] + 1 < 5){
                        questionId[i]++;
                        postazioneStep[i] = "waiting for answer";
                    } else if (questionId[i] + 1 == 5) {
                        if(postazionePoints[i] >= 60){
                            imgAnswerId[i] = 0;
                        } else {
                            imgAnswerId[i] = 1;
                        }
                        postazioneStep[i] = "showing points";
                    }
                    updateTimer(i);
                } else {
                    if(imgAnswerId[i] == postCorrectAnswer[i][questionId[i]]){
                        frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_correct_st4[i].size();
                        kid_correct_st4[i] = true;
                    } else {
                        frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_wrong_st4[i].size();
                        kid_correct_st4[i] = false;
                    }
                }
            } else if(postazioneStep[i] == "showing points"){
                if(thisElapsedTime > timeToEnjoyPoints){
                    postazioneStep[i] = "waiting touch";
                    postazioneStatus[i] = false;
                } else {
                    if(postazionePoints[i] >= 60){
                        frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_correct_st5[i].size();
                        kid_correct_st5[i] = true;
                    } else {
                        frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_wrong_st5[i].size();
                        kid_correct_st5[i] = false;
                    }
                }
            }
        } else {
            if(touchwallStatus){
                frameIndex_p123[i] = (int)(ofGetElapsedTimef() * sequenceFPS) % a_pos123_st1[i].size();
            }
        }
    }
}

void game::draw(){
    // POSTAZIONE 0
    if( !touchwallStatus ||
        (postazioneStep[0]!="pre-game" &&
         postazioneStep[1]!="pre-game" &&
         postazioneStep[2]!="pre-game")
       ){
        a_pos0_st0[frameIndex_p0].draw(postazione_0.x, postazione_0.y);
    }

    for( int i=0; i<3; i++ ){
        if(postazioneStatus[i]){
            float thisElapsedTime = ofGetElapsedTimef()-lastElapsedTime[i];
            // POSTAZIONE 1, 2, 3
            if(postazioneStep[i] == "pre-game"){
                a_pos123_st2[i][frameIndex_p123[i]].draw(0, 0);
            }
            else if(postazioneStep[i] == "waiting for answer"){
                i_pos123_st3[i][questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                
                if(kid_wait_st3[i]){
                    a_kid_wait[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else {
                    a_kid_time[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
                
                ofPushStyle();
                ofPushMatrix();
                    ofTranslate(arrow_pos[i].x + i_arrow[i].getWidth()/2, arrow_pos[i].y + i_arrow[i].getHeight()/2);//move pivot to centre
                    ofRotate(ofGetFrameNum() * .5, 0, 0, 1);//rotate from centre
                    ofPushMatrix();
                        ofTranslate(-arrow_pos[i].x-i_arrow[i].getWidth()/2,-arrow_pos[i].y-i_arrow[i].getHeight()/2);//move back by the centre offset
                        ofSetColor(255, 255, 255, 255);
                        i_arrow[i].draw(arrow_pos[i]);
                    ofPopMatrix();
                ofPopMatrix();
                ofPopStyle();
                
                timeFont.drawString(ofToString(maxAnswerTime-thisElapsedTime),
                                    postazionePos[i].x+350, postazionePos[i].y+100);
            }
            else if(postazioneStep[i] == "showing answer" ||
                    postazioneStep[i] == "showing timeout"){
                i_pos123_st4[i][imgAnswerId[i]][questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                
                if(kid_correct_st4[i]){
                    a_kid_correct_st4[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else {
                    a_kid_wrong_st4[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
            }
            else if(postazioneStep[i] == "showing points"){
                pointsFont.drawString(ofToString(postazionePoints[i]),
                                      postazionePos[i].x+350, postazionePos[i].y+400);
                
                if(kid_correct_st5[i]){
                    a_kid_correct_st5[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else {
                    a_kid_wrong_st5[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
            }
        } else {
            if(touchwallStatus){
                a_pos123_st1[i][frameIndex_p123[i]].draw(postazionePos[i].x, postazionePos[i].y);
            }
        }
    }
}

void game::startPostazione(int postId){
    postazioneStatus[postId] = true;
    postazioneStep[postId] = "pre-game";
    postazionePoints[postId] = 0;
    questionId[postId] = 0;
    frameIndex_p123[postId] = 0;
    prevFrameIndex_p123[postId] = 0;
    frameIndex_kid[postId] = 0;
    updateTimer(postId);
}

void game::updateTimer(int postId){
    lastElapsedTime[postId] = ofGetElapsedTimef();
    frameIndex_kid[postId] = 0;
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
    
    postCorrectAnswer[0][0] = 0;
    postCorrectAnswer[0][1] = 1;
    postCorrectAnswer[0][2] = 2;
    postCorrectAnswer[0][3] = 0;
    postCorrectAnswer[0][4] = 1;
    postCorrectAnswer[1][0] = 0;
    postCorrectAnswer[1][1] = 1;
    postCorrectAnswer[1][2] = 2;
    postCorrectAnswer[1][3] = 0;
    postCorrectAnswer[1][4] = 1;
    postCorrectAnswer[2][0] = 0;
    postCorrectAnswer[2][1] = 1;
    postCorrectAnswer[2][2] = 2;
    postCorrectAnswer[2][3] = 0;
    postCorrectAnswer[2][4] = 1;
}

void game::loadAllImages(){
    ofDirectory d;
    for(int state=0; state<6; state++){
        switch(state){
                
                
            // STATE 0 - 3 postazione are "waiting touch"
            case 0:
            {
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_0/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st0.push_back(ofImage());
                    a_pos0_st0[i].load(d.getPath(i));
                    a_pos0_st0[i].resize(a_pos0_st0[i].getWidth()*percent,
                                         a_pos0_st0[i].getHeight()*percent);
                }
            }
                break;
                
                
            // STATE 1 - some postazione "waiting touch"
            case 1:
            {
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_1/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st1.push_back(ofImage());
                    a_pos0_st1[i].load(d.getPath(i));
                    a_pos0_st1[i].resize(a_pos0_st1[i].getWidth()*percent,
                                         a_pos0_st1[i].getHeight()*percent);
                }
                
                // POSTAZIONE 1, 2, 3 (same animation - diff variables)
                d.listDir("media/state_1/animation/postazione_123");
                float resizeFactor[3];
                resizeFactor[0] = 1.55;
                resizeFactor[1] = 1.95;
                resizeFactor[2] = resizeFactor[0];
                for(int i=0; i<3; i++){
                    for(int j=0; j<d.size(); j++){
                        a_pos123_st1[i].push_back(ofImage());
                        a_pos123_st1[i][j].load(d.getPath(j));
                        a_pos123_st1[i][j].resize(a_pos123_st1[i][j].getWidth()*resizeFactor[i],
                                                  a_pos123_st1[i][j].getHeight()*resizeFactor[i]);
                    }
                }
            }
                break;
                
                
            // STATE 2 - "pre-game"
            case 2:
            {
                // POSTAZIONE 1, 2, 3 (different animations)
                for(int i=0; i<3; i++){
                    d.listDir("media/state_2/animation/postazione_" + ofToString(i+1));
                    for(int j=0; j<d.size(); j++){
                        a_pos123_st2[i].push_back(ofImage());
                        a_pos123_st2[i][j].load(d.getPath(j));
                    }
                }
            }
                break;
                
                
            // STATE 3 - "waiting for answer"
            case 3:
            {
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_3/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st3.push_back(ofImage());
                    a_pos0_st3[i].load(d.getPath(i));
                    a_pos0_st3[i].resize(a_pos0_st3[i].getWidth()*percent,
                                         a_pos0_st3[i].getHeight()*percent);
                }
                
                // TIME ARROW
                i_arrow[0].load("media/state_3/animation/arrow/arrow_1-3.png");
                i_arrow[1].load("media/state_3/animation/arrow/arrow_2.png");
                i_arrow[2].load("media/state_3/animation/arrow/arrow_1-3.png");
                arrow_pos[0] = ofVec2f(353, 98);
                arrow_pos[1] = ofVec2f(812, 29);
                arrow_pos[2] = ofVec2f(1395, 98);
                
                // POSTAZIONE 1, 2, 3 (question images)
                float resizeFactor[3];
                resizeFactor[0] = 1.55;
                resizeFactor[1] = 1.95;
                resizeFactor[2] = resizeFactor[0];
                for(int i=0; i<3; i++){
                    // QUESTION images
                    d.listDir("media/state_3/image/postazione_" + ofToString(i+1));
                    for(int j=0; j<d.size(); j++){
                        i_pos123_st3[i].push_back(ofImage());
                        i_pos123_st3[i][j].load(d.getPath(j));
                        i_pos123_st3[i][j].resize(i_pos123_st3[i][j].getWidth()*resizeFactor[i],
                                                  i_pos123_st3[i][j].getHeight()*resizeFactor[i]);
                    }
                    
                    // KID WAIT
                    d.listDir("media/state_3/animation/kid_0_wait");
                    for(int j=0; j<d.size(); j++){
                        a_kid_wait[i].push_back(ofImage());
                        a_kid_wait[i][j].load(d.getPath(j));
                        a_kid_wait[i][j].resize(a_kid_wait[i][j].getWidth()*resizeFactor[i],
                                                a_kid_wait[i][j].getHeight()*resizeFactor[i]);
                    }
                    
                    // KID TIME
                    d.listDir("media/state_3/animation/kid_1_time");
                    for(int j=0; j<d.size(); j++){
                        a_kid_time[i].push_back(ofImage());
                        a_kid_time[i][j].load(d.getPath(j));
                        a_kid_time[i][j].resize(a_kid_time[i][j].getWidth()*resizeFactor[i],
                                                a_kid_time[i][j].getHeight()*resizeFactor[i]);
                    }
                }
                
            }
                break;
            
            // STATE 4 - "showing answer/timeout"
            case 4:
            {
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_4/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st4.push_back(ofImage());
                    a_pos0_st4[i].load(d.getPath(i));
                    a_pos0_st4[i].resize(a_pos0_st4[i].getWidth()*percent,
                                         a_pos0_st4[i].getHeight()*percent);
                }
                
                // POSTAZIONE 1, 2, 3 (answer and timeout images)
                float resizeFactor[3];
                resizeFactor[0] = 1.55;
                resizeFactor[1] = 1.95;
                resizeFactor[2] = resizeFactor[0];
                for(int i=0; i<3; i++){
                    // ANSWER and TIMEOUT images
                    d.listDir("media/state_4/image/postazione_" + ofToString(i+1));
                    for(int j=0; j<4; j++){
                        ofDirectory d2;
                        for(int k=0; k<d.size(); k++){
                            d2.listDir("media/state_4/image/postazione_" + ofToString(i+1) +
                                       "/question_" + ofToString(k+1));
                            i_pos123_st4[i][j].push_back(ofImage());
                            i_pos123_st4[i][j][k].load(d2.getPath(j));
                            i_pos123_st4[i][j][k].resize(i_pos123_st4[i][j][k].getWidth()*resizeFactor[i],
                                                         i_pos123_st4[i][j][k].getHeight()*resizeFactor[i]);
                        }
                    }
                    
                    // KID CORRECT
                    d.listDir("media/state_4/animation/kid_correct");
                    for(int j=0; j<d.size(); j++){
                        a_kid_correct_st4[i].push_back(ofImage());
                        a_kid_correct_st4[i][j].load(d.getPath(j));
                        a_kid_correct_st4[i][j].resize(a_kid_correct_st4[i][j].getWidth()*resizeFactor[i],
                                                       a_kid_correct_st4[i][j].getHeight()*resizeFactor[i]);
                    }
                    
                    // KID WRONG
                    d.listDir("media/state_4/animation/kid_wrong");
                    for(int j=0; j<d.size(); j++){
                        a_kid_wrong_st4[i].push_back(ofImage());
                        a_kid_wrong_st4[i][j].load(d.getPath(j));
                        a_kid_wrong_st4[i][j].resize(a_kid_wrong_st4[i][j].getWidth()*resizeFactor[i],
                                                     a_kid_wrong_st4[i][j].getHeight()*resizeFactor[i]);
                    }
                }
            }
                break;
                
            // STATE 5 - "showing points"
            case 5:
            {
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_4/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st5.push_back(ofImage());
                    a_pos0_st5[i].load(d.getPath(i));
                    a_pos0_st5[i].resize(a_pos0_st5[i].getWidth()*percent,
                                         a_pos0_st5[i].getHeight()*percent);
                }
                
                // CORRECT or WRONG
                float resizeFactor[3];
                resizeFactor[0] = 1.55;
                resizeFactor[1] = 1.95;
                resizeFactor[2] = resizeFactor[0];
                for(int i=0; i<3; i++){
                    // KID CORRECT
                    d.listDir("media/state_5/animation/kid_correct");
                    for(int j=0; j<d.size(); j++){
                        a_kid_correct_st5[i].push_back(ofImage());
                        a_kid_correct_st5[i][j].load(d.getPath(j));
                        a_kid_correct_st5[i][j].resize(a_kid_correct_st5[i][j].getWidth()*resizeFactor[i],
                                                       a_kid_correct_st5[i][j].getHeight()*resizeFactor[i]);
                    }
                    
                    // KID WRONG
                    d.listDir("media/state_5/animation/kid_wrong");
                    for(int j=0; j<d.size(); j++){
                        a_kid_wrong_st5[i].push_back(ofImage());
                        a_kid_wrong_st5[i][j].load(d.getPath(j));
                        a_kid_wrong_st5[i][j].resize(a_kid_wrong_st5[i][j].getWidth()*resizeFactor[i],
                                                     a_kid_wrong_st5[i][j].getHeight()*resizeFactor[i]);
                    }
                }
                
                // POINTS IMAGES
                pointsImg[0].load("media/state_5/image/QEND_SI.png");
                pointsImg[1].load("media/state_5/image/QEND_NO.png");
            }
                break;
                
            default:
                break;
        }
        
    }
}