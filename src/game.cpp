//
//  game.cpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 04/08/17.
//
//

#include "game.h"

void game::setup(int nElect, settings gameSettings){
    
    // Bare Conductive
    nElectrodes = nElect;
    
    // Inicializar touch status
    for(int i=0; i<nElectrodes; i++){
        touchStatus.push_back(false);
    }
    
    touchwallStatus = false;
    
    loadXmlSettings(gameSettings);
    // loadXmlValuesManual();
    
    loadAllImages();
    
    postazione_0 = ofVec2f(0, 0);
    postazionePos[0] = ofVec2f(268, 0);
    postazionePos[1] = ofVec2f(268 + 500, 0);
    postazionePos[2] = ofVec2f(268 + 500 + 620 - 15, 0);
    ajustePreguntaPost3 = ofVec2f(0, 10);
    
    postazioneStatus[0] = false;
    postazioneStatus[1] = postazioneStatus[0];
    postazioneStatus[2] = postazioneStatus[0];
    
    postazioneStep[0] = "waiting touch";
    postazioneStep[1] = postazioneStep[0];
    postazioneStep[2] = postazioneStep[0];
    
    alphaPostazione[0] = 255;
    alphaPostazione[1] = 255;
    alphaPostazione[2] = 255;
    
    tiempoRestante.load("fonts/UniversLTStd-BoldCn.otf", 15);
    
    is_debugging = false;
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
                frameIndex_p123[i] = (int)(thisElapsedTime * sequenceFPS) % anim_entra[i].size();
                if(frameIndex_p123[i] == 0 && prevFrameIndex_p123[i]!=0 && !is_debugging){
                    // Cuando la animación acaba, pasamos al siguiente estado
                    postazioneStep[i] = "waiting for answer";
                    frameIndex_p123[i] = 0;
                    frameIndex_nino[i] = 0;
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
                            nino_esperando[i] = true;
                        } else {
                            nino_esperando[i] = false;
                        }
                        // Si se toca algún botón ABC
                        if(touchStatus[optionElectIndex[i][j]]){
                            // TODO: CHECK THIS
                            // Comprobar si índice de botón es válido
                            // [Para preguntas que son de dos respuestas posibles,
                            //  para poder descartar interacción del botón C]
                            if(numAnswerPerQuestion[i][questionId[i]]-1 >= j){
                                // Cargar imagen correspondiente de respuesta
                                imgAnswerId[i] = j;
                                // Comprobar si la respuesta es correcta para sumar puntuación
                                if(imgAnswerId[i] == postCorrectAnswer[i][questionId[i]]){
                                    postazionePoints[i] += 20;
                                }
                                // Cambiar estado -> enseñar la respuesta
                                postazioneStep[i] = "showing answer";
                                frameIndex_nino[i] = 0;
                                updateTimer(i);
                            }
                        }
                    }
                } else {
                    // Si se acaba el tiempo -> enseñar timeout
                    imgAnswerId[i] = 3;
                    frameIndex_nino[i] = 0;
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
                        // Si estamos en la última pregunta, comprobamos puntos
                        // y asignamos índice de imagen de puntuación
                        if(postazionePoints[i] == 0){
                            // índice para imagen 00 puntos
                            imgAnswerId[i] = 0;
                        } else if (postazionePoints[i] == 20){
                            // índice para imagen 20 puntos
                            imgAnswerId[i] = 1;
                        } else if (postazionePoints[i] == 40){
                            // índice para imagen 20 puntos
                            imgAnswerId[i] = 2;
                        } else if (postazionePoints[i] == 60){
                            // índice para imagen 20 puntos
                            imgAnswerId[i] = 3;
                        } else if (postazionePoints[i] == 80){
                            // índice para imagen 20 puntos
                            imgAnswerId[i] = 4;
                        } else if (postazionePoints[i] == 100){
                            // índice para imagen 20 puntos
                            imgAnswerId[i] = 5;
                        }
                        // cambiamos el estado
                        postazioneStep[i] = "showing points";
                    }
                    frameIndex_nino[i] = 0;
                    updateTimer(i);
                }
                // Si estamos enseñando respuesta o timeout
                else {
                    // Actualizar animación de niño correspondiente
                    // dependiendo de si la respuesta es correcta o no
                    if(imgAnswerId[i] == postCorrectAnswer[i][questionId[i]]){
                        nino_correcto[i] = true;
                    } else {
                        nino_correcto[i] = false;
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
                        nino_correcto[i] = true;
                    } else {
                        nino_correcto[i] = false;
                    }
                }
            }
        }
        // Para Postazione N=i - NO ACTIVA
        else {
            // Si otra Postazione está ACTIVA
            if(touchwallStatus){
                // Actualizar el index de animación "de llamada de atención" para Postazione N=i
                if(i==0 || i==2){
                    // Index Postazione 1 y 3
                    frameIndex_p123[i] = (int)(ofGetElapsedTimef() * sequenceFPS) % anim_salvap_1_3.size();
                } else if (i==1) {
                    // Index Postazione 2
                    frameIndex_p123[i] = (int)(ofGetElapsedTimef() * sequenceFPS) % anim_salvap_2.size();
                }
            }
        }
    }
}

void game::draw(){
    // --- State 0 - "all waiting touch" --- //
    if(!touchwallStatus){
        ofPushStyle();
        ofSetColor(255, 255, 255, 255);
        frameIndex_salvap_full = (int)(ofGetElapsedTimef() * sequenceFPS) % anim_salvap_fullscreen.size();
        anim_salvap_fullscreen[frameIndex_salvap_full].draw(postazione_0.x, postazione_0.y);
        ofPopStyle();
    }

    // Recorremos las 3 postaziones
    for( int i=0; i<3; i++ ){
        // Para Postazione N=i - ACTIVA
        if(postazioneStatus[i]){
            ofPushStyle();
            ofSetColor(255, 255, 255, 255);
            
            // Recoger elapsed time
            float thisElapsedTime = ofGetElapsedTimef()-lastElapsedTime[i];
            
            // --- State 2 - "pre-game" --- //
            if(postazioneStep[i] == "pre-game"){
                // Animación Postazione 1, 2, 3
                anim_entra[i][frameIndex_p123[i]].draw(0, 0);
            }
            
            // --- State 3 - "waiting answer" --- //
            else if(postazioneStep[i] == "waiting for answer"){
                // Imagen pregunta
                if(i==0 || i==1){
                    // Postazione 1 y 2
                    img_preguntas[i][questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else if (i==2) {
                    // Postazione 3 (hay que hacer ajuste por diferencia postazione 1 y 3 en panel)
                    img_preguntas[i][questionId[i]].draw(postazionePos[i].x + ajustePreguntaPost3.x, postazionePos[i].y + ajustePreguntaPost3.y);
                }
                
                // Imagen número de pregunta
                if(i==0 || i==2){
                    // Postazione 1, 3
                    num_pregunta_1_3[questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else if (i==1) {
                    // Postazione 2
                    num_pregunta_2[questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
                
                // Animación niño esperando / niño últimos segundos
                if(nino_esperando[i]){
                    if(i==0 || i==2) {
                        // Animación esperando Postazione 1 y 3
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_espera_1_3.size();
                        anim_espera_1_3[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    } else if(i==1) {
                        // Animación esperando Postazione 2
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_espera_2.size();
                        anim_espera_2[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                } else {
                    if(i==0 || i==2) {
                        // Animación últimos segundos Postazione 1 y 3
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_tiempo_1_3.size();
                        anim_tiempo_1_3[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    } else if(i==1) {
                        // Animación últimos segundos Postazione 2
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_tiempo_2.size();
                        anim_tiempo_2[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                }
                
                // Animación flecha tiempo
                ofPushStyle();
                ofPushMatrix();
                ofTranslate(arrow_pos[i].x + img_arrow[i].getWidth()/2, arrow_pos[i].y + img_arrow[i].getHeight()/2);//move pivot to centre
                if(i==0 || i==2){
                    ofRotate(ofMap(thisElapsedTime, 0.0, maxAnswerTime, 0, 270));
                } else if (i==1){
                    ofRotate(ofMap(thisElapsedTime, 0.0, maxAnswerTime, 0, 290));
                }
                ofPushMatrix();
                ofTranslate(-arrow_pos[i].x-img_arrow[i].getWidth()/2,-arrow_pos[i].y-img_arrow[i].getHeight()/2);//move back by the centre offset
                ofSetColor(255, 255, 255, 255);
                img_arrow[i].draw(arrow_pos[i]);
                
                // Texto tiempo restante
                if(tiempoRestActivo){
                    float marginW;
                    float marginH;
                    float extraX;
                    float extraY;
                    marginW = img_arrow[i].getWidth()/8;
                    marginH = img_arrow[i].getHeight()/8;
                    ofTranslate(arrow_pos[i].x + marginW + tiempoRestante.stringWidth(ofToString((int)(maxAnswerTime-thisElapsedTime))),
                                arrow_pos[i].y + marginH + tiempoRestante.stringHeight(ofToString((int)(maxAnswerTime-thisElapsedTime))));
                    if(i==0 || i==2){
                        if(thisElapsedTime<maxAnswerTime/3){
                            extraY = 0;
                            extraX = 0;
                        } else if (thisElapsedTime>maxAnswerTime/3 && thisElapsedTime<2*maxAnswerTime/3){
                            extraY = ofMap(thisElapsedTime, maxAnswerTime/3, 2*maxAnswerTime/3, 0, 25);
                            if(thisElapsedTime>maxAnswerTime/3 && thisElapsedTime<1.5*maxAnswerTime/3){
                                extraX = ofMap(thisElapsedTime, maxAnswerTime/3, 1.5*maxAnswerTime/3, 0, 20);
                            } else if (thisElapsedTime>1.5*maxAnswerTime/3 && thisElapsedTime<2*maxAnswerTime/3){
                                extraX = ofMap(thisElapsedTime, 1.5*maxAnswerTime/3, 2*maxAnswerTime/3, 20, 0);
                            }
                        } else {
                            extraX = 0;
                            extraY = 25;
                        }
                        ofRotate(-ofMap(thisElapsedTime, 0.0, maxAnswerTime, 0, 270));
                    } else if (i==1){
                        if(thisElapsedTime<maxAnswerTime/2){
                            extraY = 15;
                        } else {
                            extraY = 25;
                        }
                        ofRotate(-ofMap(thisElapsedTime, 0.0, maxAnswerTime, 0, 290));
                    }
                    ofTranslate(-arrow_pos[i].x - marginW - tiempoRestante.stringWidth(ofToString((int)(maxAnswerTime-thisElapsedTime))),
                                -arrow_pos[i].y - marginH - tiempoRestante.stringHeight(ofToString((int)(maxAnswerTime-thisElapsedTime))));
                    ofPushStyle();
                    ofSetColor(0, 255);
                    tiempoRestante.drawString(ofToString((int)(maxAnswerTime-thisElapsedTime)),
                                              arrow_pos[i].x + marginW + extraX, arrow_pos[i].y + marginH + extraY);
                    ofLog() << "i -> " << i;
                    ofLog() << "EXTRA X " << extraX;
                    ofLog() << "EXTRA Y " << extraY;
                    ofLog() << "this elapsed " << thisElapsedTime;
                    ofPopStyle();
                }
                ofPopMatrix();
                ofPopMatrix();
                ofPopStyle();
                
            }
            
            // --- State 4 - "showing answer or timeout" --- //
            else if(postazioneStep[i] == "showing answer" ||
                    postazioneStep[i] == "showing timeout"){
                
                // Imagen fija de respuesta
                if(postazioneStep[i] == "showing answer"){
                    if(i==0 || i==1){
                        // Imagen respuesta - p. 1, 2
                        img_respuestas[i][questionId[i]][imgAnswerId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    } else if (i==2) {
                        // Imagen respuesta - p. 3
                        // (necesita ajuste de pixeles para pregunta por posicion postazione 3 en panel)
                        img_respuestas[i][questionId[i]][imgAnswerId[i]].draw(postazionePos[i].x + ajustePreguntaPost3.x, postazionePos[i].y + ajustePreguntaPost3.y);
                    }
                }
                
                // Imagen fija cuando no hay respuesta - tiempo agotado (timeout)
                if(postazioneStep[i] == "showing timeout"){
                    // Imagen de pregunta
                    img_preguntas[i][questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    
                    // Imagen de timeout
                    if(i==0 || i==2){
                        // Timeout - p. 1, 3
                        img_tiempoagotado_1_3.draw(postazionePos[i].x, postazionePos[i].y);
                    } else if (i==1) {
                        // Timeout - p. 2
                        img_tiempoagotado_2.draw(postazionePos[i].x, postazionePos[i].y);
                    }
                }
                
                // Imagen número de pregunta
                if(i==0 || i==2){
                    // Postazione 1, 3
                    num_pregunta_1_3[questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else if (i==1) {
                    // Postazione 2
                    num_pregunta_2[questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }

                // Animación niño respuesta correcta / incorrecta
                if(nino_correcto[i]){
                    if(i==0 || i==2) {
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_correcto_1_3.size();
                        anim_correcto_1_3[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    } else if(i==1) {
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_correcto_2.size();
                        anim_correcto_2[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                } else {
                    if(i==0 || i==2) {
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_incorrecto_1_3.size();
                        anim_incorrecto_1_3[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    } else if(i==1) {
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_incorrecto_2.size();
                        anim_incorrecto_2[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                }
            }
            
            // --- State 5 - "showing points" --- //
            else if(postazioneStep[i] == "showing points"){
                // Imagen Puntuacion
                if(i==0){
                    // Postazione 1: Imagen para postazione 1 y 3
                    img_puntos_1_3[imgAnswerId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else if (i==1){
                    // Postazione 2: Imagen para postazione 2
                    img_puntos_2[imgAnswerId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else if (i==2){
                    // Postazione 3: Imagen para postazione 1 y 3
                    // TODO: *Se realiza un ajuste de pixeles para esta imagen*
                    img_puntos_1_3[imgAnswerId[i]].draw(postazionePos[i].x + ajustePreguntaPost3.x, postazionePos[i].y + ajustePreguntaPost3.y);
                }
                
                // Animación niño para puntuación aprobada / suspendida
                if(nino_correcto[i]){
                    if(i==0 || i==2) {
                        // Animación niño correcto para Postazione 1 y 3
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_correcto_1_3.size();
                        anim_correcto_1_3[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    } else if (i==1) {
                        // Animación niño correcto para Postazione 2
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_correcto_2.size();
                        anim_correcto_2[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                } else {
                    if(i==0 || i==2) {
                        // Animación niño incorrecto para Postazione 1 y 3
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_incorrecto_1_3.size();
                        anim_incorrecto_1_3[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    } else if (i==1) {
                        // Animación niño incorrecto para Postazione 2
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_incorrecto_2.size();
                        anim_incorrecto_2[frameIndex_nino[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    }
                }
            }
            ofPopStyle();
        }
        // Para Postazione N=i - NO ACTIVA
        else {
            ofPushStyle();
            ofSetColor(255, 255, 255, 255);
            // --- State 1 - "waiting touch" --- //
            // Si otra Postazione está ACTIVA
            if(touchwallStatus){
                // Salvapantallas Una Postazione
                if(i==0 || i==2) {
                    // Postazione 1 y 3
                    anim_salvap_1_3[frameIndex_p123[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else if (i==1) {
                    // Postazione 2
                    anim_salvap_2[frameIndex_p123[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
            }
            ofPopStyle();
        }
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
    frameIndex_nino[postId] = 0;
}

void game::updateTimer(int postId){
    // Actualizar last elapsed time para la Postazione N=postId
    lastElapsedTime[postId] = ofGetElapsedTimef();
    frameIndex_nino[postId] = 0;
}

void game::loadXmlSettings(settings gameSettings){
    // Tiempo restante (show?)
    tiempoRestActivo = gameSettings.showRemainingTime;
    
    // FPS Secuencia
    sequenceFPS = gameSettings.sequenceFPS;
    
    // Tiempos
    maxAnswerTime = gameSettings.maxAnswerTime;
    lastSecondsAmount = gameSettings.lastSecondsTime;
    timeToNextQuestion = gameSettings.toNextQuestionTime;
    timeToEnjoyPoints = gameSettings.pointsTime;
    
    // Puntos
    pointsToPass = gameSettings.pointsToPass;
    
    // Panel
    for(int i=0; i<3; i++){
        // Botones Postazione -> Index Electrodo
        postElectIndex[i] = gameSettings.postazioneElectIndex[i];
        
        for(int j=0; j<5; j++){
            // Botones Opciones -> Index Electrodo
            optionElectIndex[i][j] = gameSettings.abcElectIndex[i][j];
            // Índice de respuestas correctas
            postCorrectAnswer[i][j] = gameSettings.postCorrectAnswer[i][j];
            // Número de respuestas por pregunta
            numAnswerPerQuestion[i][j] = gameSettings.numAnswerPerQuestion[i][j];
        }
    }
}

void game::loadXmlValuesManual(){
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
    postCorrectAnswer[0][0] = 0;
    postCorrectAnswer[0][1] = 1;
    postCorrectAnswer[0][2] = 2;
    postCorrectAnswer[0][3] = 0;
    postCorrectAnswer[0][4] = 1;
    postCorrectAnswer[1][0] = 1;
    postCorrectAnswer[1][1] = 2;
    postCorrectAnswer[1][2] = 0;
    postCorrectAnswer[1][3] = 2;
    postCorrectAnswer[1][4] = 1;
    postCorrectAnswer[2][0] = 0;
    postCorrectAnswer[2][1] = 1;
    postCorrectAnswer[2][2] = 2;
    postCorrectAnswer[2][3] = 1;
    postCorrectAnswer[2][4] = 1;
    
    // Número de respuestas por pregunta
    numAnswerPerQuestion[0][0] = 3;
    numAnswerPerQuestion[0][1] = 3;
    numAnswerPerQuestion[0][2] = 3;
    numAnswerPerQuestion[0][3] = 3;
    numAnswerPerQuestion[0][4] = 3;
    numAnswerPerQuestion[1][0] = 3;
    numAnswerPerQuestion[1][1] = 3;
    numAnswerPerQuestion[1][2] = 3;
    numAnswerPerQuestion[1][3] = 3;
    numAnswerPerQuestion[1][4] = 3;
    numAnswerPerQuestion[2][0] = 2;
    numAnswerPerQuestion[2][1] = 2;
    numAnswerPerQuestion[2][2] = 3;
    numAnswerPerQuestion[2][3] = 3;
    numAnswerPerQuestion[2][4] = 3;
    
    // Tiempos
    maxAnswerTime = 15;
    lastSecondsAmount = 5;
    timeToNextQuestion = 5;
    timeToEnjoyPoints = 10;
    
    // Puntos
    pointsToPass = 60;
    
    // Tiempo restante (show?)
    tiempoRestActivo = false;
    
    // FPS Secuencia
    sequenceFPS = 18;
}

// Cargar todas las imágenes y animaciones necesarias
void game::loadAllImages(){

    // --- MEDIA ASOCIADO A VARIOS ESTADOS --- //
    // Animación correcto / incorrecto
    ofDirectory dV;
    dV.listDir("media/animacion/nino_correcto/postazione_1_3");
    for(int i=0; i<dV.size(); i++){
        ofFile file(ofToDataPath(dV.getPath(i)));
        if(file.getExtension() == "png"){
            anim_correcto_1_3.push_back(dV.getPath(i));
        }
    }
    dV.listDir("media/animacion/nino_correcto/postazione_2");
    for(int i=0; i<dV.size(); i++){
        ofFile file(ofToDataPath(dV.getPath(i)));
        if(file.getExtension() == "png"){
            anim_correcto_2.push_back(dV.getPath(i));
        }
    }
    dV.listDir("media/animacion/nino_incorrecto/postazione_1_3");
    for(int i=0; i<dV.size(); i++){
        ofFile file(ofToDataPath(dV.getPath(i)));
        if(file.getExtension() == "png"){
            anim_incorrecto_1_3.push_back(dV.getPath(i));
        }
    }
    dV.listDir("media/animacion/nino_incorrecto/postazione_2");
    for(int i=0; i<dV.size(); i++){
        ofFile file(ofToDataPath(dV.getPath(i)));
        if(file.getExtension() == "png"){
            anim_incorrecto_2.push_back(dV.getPath(i));
        }
    }

    
    // --- MEDIA ASOCIADO A UN SOLO ESTADO --- //
    ofDirectory d;
    for(int state=0; state<6; state++){
        switch(state){
                
            // STATE 0 - 3 postazione are "waiting touch"
            case 0:
            {
                // Animación Salvapantallas Fullscreen
                d.listDir("media/animacion/salvapantallas_fullscreen");
                for(int i=0; i<d.size(); i++){
                    anim_salvap_fullscreen.push_back(ofImage());
                    anim_salvap_fullscreen[i].load(d.getPath(i));
                }
            }
                break;
                
                
            // STATE 1 - some postazione "waiting touch"
            case 1:
            {
                // Animación Salvapantallas Niño Una Postazione
                d.listDir("media/animacion/salvapantallas_postazione/postazione_1_3");
                for(int i=0; i<d.size(); i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        anim_salvap_1_3.push_back(d.getPath(i));
                    }
                }
                d.listDir("media/animacion/salvapantallas_postazione/postazione_2");
                for(int i=0; i<d.size(); i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        anim_salvap_2.push_back(d.getPath(i));
                    }
                }
            }
                break;
                
                
            // STATE 2 - "pre-game"
            case 2:
            {
                // POSTAZIONE 1, 2, 3 (different animations)
                for(int i=0; i<3; i++){
                    d.listDir("media/animacion/nino_entra/postazione_" + ofToString(i+1));
                    for(int j=0; j<d.size(); j++){
                        ofFile file(ofToDataPath(d.getPath(j)));
                        if(file.getExtension() == "png"){
                            anim_entra[i].push_back(ofImage());
                            anim_entra[i][j].load(d.getPath(j));
                        }
                    }
                }
            }
                break;
                
                
            // STATE 3 - "waiting for answer"
            case 3:
            {
                // Imagen Preguntas
                for(int i=0; i<3; i++){
                    d.listDir("media/imagenFija/preguntas/postazione_" + ofToString(i+1));
                    for(int j=0; j<d.size(); j++){
                        ofFile file(ofToDataPath(d.getPath(j)));
                        if(file.getExtension() == "png"){
                            img_preguntas[i][j].load(d.getPath(j));
                        }
                    }
                }
                
                // Imagen Número de Pregunta
                d.listDir("media/imagenFija/numPregunta/postazione_1_3");
                for(int i=0; i<d.size(); i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        num_pregunta_1_3[i].load(d.getPath(i));
                    }
                }
                d.listDir("media/imagenFija/numPregunta/postazione_2");
                for(int i=0; i<d.size(); i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        num_pregunta_2[i].load(d.getPath(i));
                    }
                }
                
                // Animación niño esperando
                d.listDir("media/animacion/nino_esperando/postazione_1_3");
                for(int i=0; i<d.size(); i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        anim_espera_1_3.push_back(d.getPath(i));
                    }
                }
                d.listDir("media/animacion/nino_esperando/postazione_2");
                for(int i=0; i<d.size(); i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        anim_espera_2.push_back(d.getPath(i));
                    }
                }
                
                // Animación niño últimos segundos
                d.listDir("media/animacion/nino_ultimosSegundos/postazione_1_3");
                for(int i=0; i<d.size(); i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        anim_tiempo_1_3.push_back(d.getPath(i));
                    }
                }
                d.listDir("media/animacion/nino_ultimosSegundos/postazione_2");
                for(int i=0; i<d.size(); i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        anim_tiempo_2.push_back(d.getPath(i));
                    }
                }
                              
                // Flecha de tiempo
                img_arrow[0].load("media/animacion/arrow/arrow_1-3.png");
                img_arrow[1].load("media/animacion/arrow/arrow_2.png");
                img_arrow[2].load("media/animacion/arrow/arrow_1-3.png");
                arrow_pos[0] = ofVec2f(288, 78);
                arrow_pos[1] = ofVec2f(770, 5);
                arrow_pos[2] = ofVec2f(1385, 78);
            }
                break;
            
            // STATE 4 - "showing answer/timeout"
            case 4:
            {
                // Imagen Respuestas
                for(int i=0; i<3; i++){
                    for(int j=0; j<5; j++){
                        d.listDir("media/imagenFija/respuestas/postazione_" + ofToString(i+1) +
                                  "/question_" + ofToString(j+1));
                        for(int k=0; k<d.size(); k++){
                            ofFile file(ofToDataPath(d.getPath(k)));
                            if(file.getExtension() == "png"){
                                img_respuestas[i][j][k].load(d.getPath(k));
                            }
                        }
                    }
                }
                // Imagen Tiempo Agotado
                img_tiempoagotado_1_3.load("media/imagenFija/tiempoAgotado/postazione_1_3/timeout.png");
                img_tiempoagotado_2.load("media/imagenFija/tiempoAgotado/postazione_2/timeout.png");
            }
                break;
                
            // STATE 5 - "showing points"
            case 5:
            {
                // Imagen Puntuacion Final
                d.listDir("media/imagenFija/puntuacion/postazione_1_3");
                for(int i=0; i<6; i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        img_puntos_1_3[i].load(d.getPath(i));
                    }
                }
                d.listDir("media/imagenFija/puntuacion/postazione_2");
                for(int i=0; i<6; i++){
                    ofFile file(ofToDataPath(d.getPath(i)));
                    if(file.getExtension() == "png"){
                        img_puntos_2[i].load(d.getPath(i));
                    }
                }
            }
                break;
                
            default:
                break;
        }
        
    }
}