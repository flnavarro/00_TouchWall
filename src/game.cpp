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
    
    touchwallStatus = false;
    
    loadButtons();
    
    sequenceFPS = 24;
    loadAllImages();
    
    postazione_0 = ofVec2f(7, 113);
    postazionePos[0] = ofVec2f(305, 55);
    postazionePos[1] = ofVec2f(750, -25);
    postazionePos[2] = ofVec2f(1350, 55);
    postazione_0_NEW = ofVec2f(0, 0);
    postazionePos_NEW[0] = ofVec2f(360, 0);
    postazionePos_NEW[1] = ofVec2f(360 + 490, 0);
    postazionePos_NEW[2] = ofVec2f(360 + 490 + 580 - 25, 0);
    
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
    
    // TODO: NOT USED
    alphaPostazione[0] = 255;
    alphaPostazione[1] = 255;
    alphaPostazione[2] = 255;
    
    timeFont.load("fonts/Verdana.ttf", 10);
    pointsFont.load("fonts/Verdana.ttf", 60);
}

void game::update(vector<bool> touchStatus){
    // --- State 0 - "all waiting touch" --- //
    if(!postazioneStatus[0] && !postazioneStatus[1] && !postazioneStatus[2]){
        touchwallStatus = false;
    }
    // --- State 1 - "waiting touch" --- //
    else {
        touchwallStatus = true;
    }
    
    // --- State 1 - "waiting touch" --- //
    if( !touchwallStatus ||
        (postazioneStep[0]!="pre-game" &&
         postazioneStep[1]!="pre-game" &&
         postazioneStep[2]!="pre-game")
       ){
        frameIndex_p0 = (int)(ofGetElapsedTimef() * sequenceFPS) % a_pos0_st0.size();
    }
    
    // Recorremos las 3 postaziones
    for( int i=0; i<3; i++ ){
        
        // --- State 1 - "waiting touch" --- //
        // Si se toca el botón touch de Postazione y el juego no está activo
        if(!postazioneStatus[i] && touchStatus[postElectIndex[i]] && !is_debugging){
            // Comenzar juego
            startPostazione(i);
            updateTimer(i);
        }
        
        // Para Postazione N=i - ACTIVA
        if(postazioneStatus[i]){
            
            // Recoger elapsed time
            float thisElapsedTime = ofGetElapsedTimef() - lastElapsedTime[i];
            
            // --- State 2 - "pre-game" --- //
            if(postazioneStep[i] == "pre-game"){
                // Actualizar animación pre-game
                frameIndex_p123[i] = (int)(thisElapsedTime * sequenceFPS) % a_pos123_st2[i].size();
                if(frameIndex_p123[i] == 0 && prevFrameIndex_p123[i]!=0 && !is_debugging){
                    // Cuando la animación acaba, pasamos al siguiente estado
                    postazioneStep[i] = "waiting for answer";
                    frameIndex_p123[i] = 0;
                    prevFrameIndex_p123[i] = 0;
                    updateTimer(i);
                }
                prevFrameIndex_p123[i] = frameIndex_p123[i];
            }
            
            // --- State 3 - "waiting answer" --- //
            else if(postazioneStep[i] == "waiting for answer"){
                if(thisElapsedTime < maxAnswerTime){
                    // Recorremos los 3 botones ABC
                    for(int j=0; j<3; j++){
                        // Actualizamos animación niño
                        if(thisElapsedTime < maxAnswerTime-lastSecondsAmount){
                            frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_wait[i].size();
                            kid_wait_st3[i] = true;
                        } else {
                            frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_time[i].size();
                            kid_wait_st3[i] = false;
                        }
                        // Si se toca algún botón ABC
                        if(touchStatus[optionElectIndex[i][j]]){
                            // Cargar imagen correspondiente de respuesta
                            imgAnswerId[i] = j;
                            // Comprobar si la respuesta es correcta para sumar puntuación
                            if(imgAnswerId[i] == postCorrectAnswer[i][questionId[i]]){
                                postazionePoints[i] += 20;
                            }
                            // Cambiar estado -> enseñar la respuesta
                            postazioneStep[i] = "showing answer";
                            updateTimer(i);
                        }
                    }
                } else {
                    // Si se acaba el tiempo -> enseñar timeout
                    imgAnswerId[i] = 3;
                    postazioneStep[i] = "showing timeout";
                    updateTimer(i);
                }
            }
            
            // --- State 4 - "showing answer or timeout" --- //
            else if(postazioneStep[i] == "showing answer" ||
                      postazioneStep[i] == "showing timeout"){
                // Si el tiempo de enseñar respuesta o timeout acaba
                if(ofGetElapsedTimef()-lastElapsedTime[i] > timeToNextQuestion){
                    if(questionId[i] + 1 < 5){
                        // Si no estamos en la última pregunta,
                        // pasamos al siguiente index de pregunta y cambiamos el estado
                        questionId[i]++;
                        postazioneStep[i] = "waiting for answer";
                    } else if (questionId[i] + 1 == 5) {
                        // Si estamos en la última pregunta,
                        // comprobamos si los puntos son suficientes para el aprobado
                        if(postazionePoints[i] >= pointsToPass){
                            // índice para imagen copa
                            imgAnswerId[i] = 0;
                        } else {
                            // índice para imagen libro
                            imgAnswerId[i] = 1;
                        }
                        // cambiamos el estado
                        postazioneStep[i] = "showing points";
                    }
                    updateTimer(i);
                }
                // Si estamos enseñando respuesta o timeout
                else {
                    // Actualizar animación de niño correspondiente
                    if(imgAnswerId[i] == postCorrectAnswer[i][questionId[i]]){
                        frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_correct_st4[i].size();
                        kid_correct_st4[i] = true;
                    } else {
                        frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_wrong_st4[i].size();
                        kid_correct_st4[i] = false;
                    }
                }
            }
            
            // --- State 5 - "showing points" --- //
            else if(postazioneStep[i] == "showing points"){
                // Si el tiempo de enseñar la puntuación termina
                if(thisElapsedTime > timeToEnjoyPoints){
                    // Desactivamos la Postazione
                    postazioneStep[i] = "waiting touch";
                    postazioneStatus[i] = false;
                }
                // Si estamos enseñando la puntuación
                else {
                    // Actualizar animación de niño correspondiente
                    if(postazionePoints[i] >= pointsToPass){
                        frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_correct_st5[i].size();
                        kid_correct_st5[i] = true;
                    } else {
                        frameIndex_kid[i] = (int)(thisElapsedTime * sequenceFPS) % a_kid_wrong_st5[i].size();
                        kid_correct_st5[i] = false;
                    }
                }
            }
        }
        // Para Postazione N=i - NO ACTIVA
        else {
            // Si otra Postazione está ACTIVA
            if(touchwallStatus){
                // Actualizar el index de animación "de llamada de atención" para Postazione N=i
                frameIndex_p123[i] = (int)(ofGetElapsedTimef() * sequenceFPS) % a_pos123_st1[i].size();
            }
        }
    }
}

void game::draw(){
    if( !touchwallStatus ||
        (postazioneStep[0]!="pre-game" &&
         postazioneStep[1]!="pre-game" &&
         postazioneStep[2]!="pre-game")
       ){
        if(a_pos0_st0_NEW){
            // [ DRAW POSITION PARA VERSIÓN FINAL ]
            a_pos0_st0[frameIndex_p0].draw(postazione_0_NEW.x, postazione_0_NEW.y);
        } else {
            a_pos0_st0[frameIndex_p0].draw(postazione_0.x, postazione_0.y);
        }
    }

    // Recorremos las 3 postaziones
    for( int i=0; i<3; i++ ){
        
        ofPushStyle();
        ofSetColor(255, 255, 255, 255);
        // Para Postazione N=i - ACTIVA
        if(postazioneStatus[i]){
            
            // Recoger elapsed time
            float thisElapsedTime = ofGetElapsedTimef()-lastElapsedTime[i];
            
            // --- State 2 - "pre-game" --- //
            if(postazioneStep[i] == "pre-game"){
                // Animación Postazione 1, 2, 3
                a_pos123_st2[i][frameIndex_p123[i]].draw(0, 0);
            }
            
            // --- State 3 - "waiting answer" --- //
            else if(postazioneStep[i] == "waiting for answer"){
                // Imagen pregunta - p. 1, 2, 3
                if(i_pos123_st3_NEW){
                    // [ DRAW POSITION PARA VERSIÓN FINAL ]
                    i_pos123_st3[i][questionId[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                } else {
                    i_pos123_st3[i][questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
                
                // Animación niño esperando / niño últimos segundos
                if(kid_wait_st3[i]){
                    if(a_kid_wait_NEW){
                        // [ DRAW POSITION PARA VERSIÓN FINAL ]
                        a_kid_wait[i][frameIndex_kid[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                    } else {
                        a_kid_wait[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                } else {
                    if(a_kid_time_NEW){
                        // [ DRAW POSITION PARA VERSIÓN FINAL ]
                        a_kid_time[i][frameIndex_kid[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                    } else {
                        a_kid_time[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                }
                
                // Animación flecha tiempo
                ofPushStyle();
                ofPushMatrix();
                    ofTranslate(arrow_pos[i].x + i_arrow[i].getWidth()/2, arrow_pos[i].y + i_arrow[i].getHeight()/2);//move pivot to centre
                    if(i==0 || i==2){
                        ofRotate(ofMap(thisElapsedTime, 0.0, 15.0, 0, 270));
                    } else if (i==1){
                        ofRotate(ofMap(thisElapsedTime, 0.0, 15.0, 0, 290));
                    }
                    ofPushMatrix();
                        ofTranslate(-arrow_pos[i].x-i_arrow[i].getWidth()/2,-arrow_pos[i].y-i_arrow[i].getHeight()/2);//move back by the centre offset
                        ofSetColor(255, 255, 255, 255);
                        i_arrow[i].draw(arrow_pos[i]);
                    ofPopMatrix();
                ofPopMatrix();
                ofPopStyle();
                
                // Texto tiempo restante
                timeFont.drawString(ofToString(maxAnswerTime-thisElapsedTime),
                                    postazionePos[i].x+350, postazionePos[i].y+100);
            }
            
            // --- State 4 - "showing answer or timeout" --- //
            else if(postazioneStep[i] == "showing answer" ||
                    postazioneStep[i] == "showing timeout"){
                // Imagen respuesta - p. 1, 2, 3
                if(i_pos123_st4_NEW){
                    // [ DRAW POSITION PARA VERSIÓN FINAL ]
                    i_pos123_st4[i][imgAnswerId[i]][questionId[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                } else {
                    i_pos123_st4[i][imgAnswerId[i]][questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
                
                // Animación niño respuesta correcta / incorrecta
                if(kid_correct_st4[i]){
                    // [ YA OK - VERSIÓN FINAL ]
                    a_kid_correct_st4[i][frameIndex_kid[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                } else {
                    if(a_kid_wrong_st4_NEW){
                        // [ DRAW POSITION PARA VERSIÓN FINAL ]
                        a_kid_wrong_st4[i][frameIndex_kid[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                    } else {
                        a_kid_wrong_st4[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                }
            }
            
            // --- State 5 - "showing points" --- //
            else if(postazioneStep[i] == "showing points"){
                // Imagen copa / libro
                if(i_points_img_NEW){
                    // [ DRAW POSITION PARA VERSIÓN FINAL ]
                    i_points_img[i][imgAnswerId[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                } else {
                    i_points_img[i][imgAnswerId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
                
                // Animación niño puntuación aprobado / suspendido
                if(kid_correct_st5[i]){
                    // [ YA OK - VERSIÓN FINAL ]
                    a_kid_correct_st5[i][frameIndex_kid[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                } else {
                    if(a_kid_wrong_st5_NEW){
                        // [ DRAW POSITION PARA VERSIÓN FINAL ]
                        a_kid_wrong_st5[i][frameIndex_kid[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                    } else {
                        a_kid_wrong_st5[i][frameIndex_kid[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                }
                
                // Texto puntos
                pointsFont.drawString(ofToString(postazionePoints[i]),
                                      postazionePos[i].x+350, postazionePos[i].y+400);
            }
        }
        // Para Postazione N=i - NO ACTIVA
        else {
            // Si otra Postazione está ACTIVA
            if(touchwallStatus){
                // Draw animación "de llamada de atención" para Postazione N=i
                if(!a_pos123_st1_NEW){
                    a_pos123_st1[i][frameIndex_p123[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else {
                    a_pos123_st1[i][frameIndex_p123[i]].draw(postazionePos_NEW[i].x, postazionePos_NEW[i].y);
                }
            }
        }
        ofPopStyle();
    }
}

void game::startPostazione(int postId){
    // Activar la Postazione N=postId e inicializar variables asociadas
    // Pasamos de State 1 (waiting touch) a State 2 (pre-game)
    postazioneStatus[postId] = true;
    postazioneStep[postId] = "pre-game";
    postazionePoints[postId] = 0;
    questionId[postId] = 0;
    frameIndex_p123[postId] = 0;
    prevFrameIndex_p123[postId] = 0;
    frameIndex_kid[postId] = 0;
}

void game::updateTimer(int postId){
    // Actualizar last elapsed time para la Postazione N=postId
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
    
    // Índice de respuestas correctas
    // TODO: Pasar a XML
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
    // Cargar todas las imágenes y animaciones necesarias
    ofDirectory d;
    for(int state=0; state<6; state++){
        switch(state){
                
                
            // STATE 0 - 3 postazione are "waiting touch"
            case 0:
            {
                // VERSIÓN DEFINITIVA ?
                a_pos0_st0_NEW = false;

                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_0/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st0.push_back(ofImage());
                    a_pos0_st0[i].load(d.getPath(i));
                    if(!a_pos0_st0_NEW){
                        a_pos0_st0[i].resize(a_pos0_st0[i].getWidth()*percent,
                                             a_pos0_st0[i].getHeight()*percent);
                    }
                }
            }
                break;
                
                
            // STATE 1 - some postazione "waiting touch"
            case 1:
            {
                // VERSIÓN DEFINITIVA ?
                a_pos0_st1_NEW = false;
                
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_1/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st1.push_back(ofImage());
                    a_pos0_st1[i].load(d.getPath(i));
                    if(!a_pos0_st1_NEW){
                        a_pos0_st1[i].resize(a_pos0_st1[i].getWidth()*percent,
                                             a_pos0_st1[i].getHeight()*percent);
                    }
                }
                
                // VERSIÓN DEFINITIVA ?
                a_pos123_st1_NEW = false;
                
                // POSTAZIONE 1, 2, 3 (same animation - diff variables)
                string folderKidAnimation[3];
                folderKidAnimation[0] = "media/state_1/animation/postazione_13";
                folderKidAnimation[1] = "media/state_1/animation/postazione_2";
                folderKidAnimation[2] = folderKidAnimation[0];
                float resizeFactor[3];
                resizeFactor[0] = 1.55;
                resizeFactor[1] = 1.95;
                resizeFactor[2] = resizeFactor[0];
                if(!a_pos123_st1_NEW){
                    d.listDir("media/state_1/animation/postazione_123");
                }
                for(int i=0; i<3; i++){
                    if(a_pos123_st1_NEW){
                        d.listDir(folderKidAnimation[i]);
                    }
                    for(int j=0; j<d.size(); j++){
                        a_pos123_st1[i].push_back(ofImage());
                        a_pos123_st1[i][j].load(d.getPath(j));
                        if(!a_pos123_st1_NEW){
                            a_pos123_st1[i][j].resize(a_pos123_st1[i][j].getWidth()*resizeFactor[i],
                                                      a_pos123_st1[i][j].getHeight()*resizeFactor[i]);
                        }
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
                // VERSIÓN FINAL ?
                a_pos0_st3_NEW = false;
                
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_3/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st3.push_back(ofImage());
                    a_pos0_st3[i].load(d.getPath(i));
                    if(!a_pos0_st3_NEW){
                        a_pos0_st3[i].resize(a_pos0_st3[i].getWidth()*percent,
                                             a_pos0_st3[i].getHeight()*percent);
                    }
                }
                
                // TIME ARROW
                i_arrow[0].load("media/state_3/animation/arrow/arrow_1-3.png");
                i_arrow[1].load("media/state_3/animation/arrow/arrow_2.png");
                i_arrow[2].load("media/state_3/animation/arrow/arrow_1-3.png");
                arrow_pos[0] = ofVec2f(353, 98);
                arrow_pos[1] = ofVec2f(812, 29);
                arrow_pos[2] = ofVec2f(1395, 98);
                
                // VERSIÓN FINAL ?
                i_pos123_st3_NEW = false;
                a_kid_wait_NEW = false;
                a_kid_time_NEW = false;
                
                // POSTAZIONE 1, 2, 3 (question images)
                float resizeFactor[3];
                resizeFactor[0] = 1.55;
                resizeFactor[1] = 1.95;
                resizeFactor[2] = resizeFactor[0];
                string folderKidWait[3];
                folderKidWait[0] = "media/state_3/animation/kid_0_wait_13";
                folderKidWait[1] = "media/state_3/animation/kid_0_wait_2";
                folderKidWait[2] = folderKidWait[0];
                string folderKidTime[3];
                folderKidTime[0] = "media/state_3/animation/kid_1_time_13";
                folderKidTime[1] = "media/state_3/animation/kid_1_time_2";
                folderKidTime[2] = folderKidTime[0];
                
                for(int i=0; i<3; i++){
                    // QUESTION images
                    d.listDir("media/state_3/image/postazione_" + ofToString(i+1));
                    for(int j=0; j<d.size(); j++){
                        i_pos123_st3[i].push_back(ofImage());
                        i_pos123_st3[i][j].load(d.getPath(j));
                        if(!i_pos123_st3_NEW){
                            i_pos123_st3[i][j].resize(i_pos123_st3[i][j].getWidth()*resizeFactor[i],
                                                      i_pos123_st3[i][j].getHeight()*resizeFactor[i]);
                        }
                    }
                    
                    // KID WAIT
                    if(!a_kid_wait_NEW){
                        d.listDir("media/state_3/animation/kid_0_wait");
                    }
                    for(int j=0; j<d.size(); j++){
                        if(a_kid_wait_NEW){
                            d.listDir(folderKidWait[0]);
                        }
                        a_kid_wait[i].push_back(ofImage());
                        a_kid_wait[i][j].load(d.getPath(j));
                        if(!a_kid_wait_NEW){
                            a_kid_wait[i][j].resize(a_kid_wait[i][j].getWidth()*resizeFactor[i],
                                                    a_kid_wait[i][j].getHeight()*resizeFactor[i]);
                        }
                    }
                    
                    // KID TIME
                    if(!a_kid_time_NEW){
                        d.listDir("media/state_3/animation/kid_1_time");
                    }
                    for(int j=0; j<d.size(); j++){
                        if(a_kid_time_NEW){
                            d.listDir(folderKidTime[0]);
                        }
                        a_kid_time[i].push_back(ofImage());
                        a_kid_time[i][j].load(d.getPath(j));
                        if(!a_kid_time_NEW){
                            a_kid_time[i][j].resize(a_kid_time[i][j].getWidth()*resizeFactor[i],
                                                    a_kid_time[i][j].getHeight()*resizeFactor[i]);
                        }
                    }
                }
                
            }
                break;
            
            // STATE 4 - "showing answer/timeout"
            case 4:
            {
                // VERSIÓN FINAL ?
                a_pos0_st4_NEW = false;
                
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_4/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st4.push_back(ofImage());
                    a_pos0_st4[i].load(d.getPath(i));
                    if(!a_pos0_st4_NEW){
                        a_pos0_st4[i].resize(a_pos0_st4[i].getWidth()*percent,
                                             a_pos0_st4[i].getHeight()*percent);
                    }
                }
                
                // VERSIÓN FINAL ?
                i_pos123_st4_NEW = false;
                a_kid_wrong_st4_NEW = false;
                
                // POSTAZIONE 1, 2, 3 (answer and timeout images)
                float resizeFactor[3];
                resizeFactor[0] = 1.55;
                resizeFactor[1] = 1.95;
                resizeFactor[2] = resizeFactor[0];
                string folderCorrect[3];
                folderCorrect[0] = "media/state_4/animation/kid_correct_13";
                folderCorrect[1] = "media/state_4/animation/kid_correct_2";
                folderCorrect[2] = folderCorrect[0];
                string folderWrong[3];
                folderWrong[0] = "media/state_4/animation/kid_wrong_13";
                folderWrong[1] = "media/state_4/animation/kid_wrong_2";
                folderWrong[2] = folderWrong[0];
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
                            if(!i_pos123_st4_NEW){
                                i_pos123_st4[i][j][k].resize(i_pos123_st4[i][j][k].getWidth()*resizeFactor[i],
                                                             i_pos123_st4[i][j][k].getHeight()*resizeFactor[i]);
                            }
                        }
                    }
                    
                    // KID CORRECT
                    d.listDir(folderCorrect[i]);
                    for(int j=0; j<d.size(); j++){
                        a_kid_correct_st4[i].push_back(ofImage());
                        a_kid_correct_st4[i][j].load(d.getPath(j));
                    }
                    
                    // KID WRONG
                    if(!a_kid_wrong_st4_NEW){
                        d.listDir("media/state_4/animation/kid_wrong");
                    } else {
                        d.listDir(folderWrong[i]);
                    }
                    for(int j=0; j<d.size(); j++){
                        a_kid_wrong_st4[i].push_back(ofImage());
                        a_kid_wrong_st4[i][j].load(d.getPath(j));
                        if(!a_kid_wrong_st4_NEW){
                            a_kid_wrong_st4[i][j].resize(a_kid_wrong_st4[i][j].getWidth()*resizeFactor[i],
                                                         a_kid_wrong_st4[i][j].getHeight()*resizeFactor[i]);
                        }
                    }
                }
            }
                break;
                
            // STATE 5 - "showing points"
            case 5:
            {
                // VERSIÓN FINAL ?
                a_pos0_st5_NEW = false;
                
                // POSTAZIONE 0
                float percent = 1.1;
                d.listDir("media/state_4/animation/postazione_0");
                for(int i=0; i<d.size(); i++){
                    a_pos0_st5.push_back(ofImage());
                    a_pos0_st5[i].load(d.getPath(i));
                    if(!a_pos0_st5_NEW){
                        a_pos0_st5[i].resize(a_pos0_st5[i].getWidth()*percent,
                                             a_pos0_st5[i].getHeight()*percent);
                    }
                }
                
                // VERSIÓN FINAL ?
                i_points_img_NEW = false;
                a_kid_wrong_st5_NEW = false;
                
                // CORRECT or WRONG
                float resizeFactor[3];
                resizeFactor[0] = 1.55;
                resizeFactor[1] = 1.95;
                resizeFactor[2] = resizeFactor[0];
                string folderCorrect[3];
                folderCorrect[0] = "media/state_5/animation/kid_correct_13";
                folderCorrect[1] = "media/state_5/animation/kid_correct_2";
                folderCorrect[2] = folderCorrect[0];
                string folderWrong[3];
                folderWrong[0] = "media/state_5/animation/kid_wrong_13";
                folderWrong[1] = "media/state_5/animation/kid_wrong_2";
                folderWrong[2] = folderWrong[0];
                for(int i=0; i<3; i++){
                    // KID CORRECT
                    d.listDir(folderCorrect[i]);
                    for(int j=0; j<d.size(); j++){
                        a_kid_correct_st5[i].push_back(ofImage());
                        a_kid_correct_st5[i][j].load(d.getPath(j));
                    }
                    
                    // KID WRONG
                    if(!a_kid_wrong_st5_NEW){
                        d.listDir("media/state_5/animation/kid_wrong");
                    } else {
                        d.listDir(folderWrong[i]);
                    }
                    for(int j=0; j<d.size(); j++){
                        a_kid_wrong_st5[i].push_back(ofImage());
                        a_kid_wrong_st5[i][j].load(d.getPath(j));
                        if(!a_kid_wrong_st5_NEW){
                            a_kid_wrong_st5[i][j].resize(a_kid_wrong_st5[i][j].getWidth()*resizeFactor[i],
                                                         a_kid_wrong_st5[i][j].getHeight()*resizeFactor[i]);
                        }
                    }
                }
                
                // POINTS IMAGES
                for(int i=0; i<3; i++){
                    i_points_img[i][0].load("media/state_5/image/QEND_SI.png");
                    i_points_img[i][1].load("media/state_5/image/QEND_NO.png");
                    if(!i_points_img_NEW){
                        i_points_img[i][0].resize(i_points_img[i][0].getWidth()*resizeFactor[i],
                                                  i_points_img[i][0].getHeight()*resizeFactor[i]);
                        i_points_img[i][1].resize(i_points_img[i][1].getWidth()*resizeFactor[i],
                                                  i_points_img[i][1].getHeight()*resizeFactor[i]);
                    }
                }
            }
                break;
                
            default:
                break;
        }
        
    }
}