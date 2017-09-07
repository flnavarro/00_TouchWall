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
    postazionePos[0] = ofVec2f(290, -20);
    postazionePos[1] = ofVec2f(300 + 475, -20);
    postazionePos[2] = ofVec2f(300 + 475 + 570, -20);
    
    anim_pos[0] = ofVec2f(postazionePos[0].x + 25, postazionePos[0].y + 60);
    anim_pos[1] = ofVec2f(postazionePos[1].x + 15, postazionePos[1].y + 60);
    anim_pos[2] = ofVec2f(postazionePos[2].x + 25, postazionePos[2].y + 60);
    
    num_pregunta_pos[0] = ofVec2f(postazionePos[0].x + 10, postazionePos[0].y);
    num_pregunta_pos[1] = ofVec2f(postazionePos[1].x, postazionePos[1].y);
    num_pregunta_pos[2] = ofVec2f(postazionePos[2].x + 10, postazionePos[2].y);
    
    salvap_peq_pos[0] = ofVec2f(525, 150);
    salvap_peq_pos[1] = ofVec2f(525 + 500, 200);
    salvap_peq_pos[2] = ofVec2f(525 + 475 + 570, 150);
    
    arrow_pos[0] = ofVec2f(310, 58);
    arrow_pos[1] = ofVec2f(765, -12);
    arrow_pos[2] = ofVec2f(1355, 58);
    
    ajusteAnimTiempoPost2 = ofVec2f(-15, 0);
    
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
    // Si no hay ninguna postazione activa
    if(!postazioneStatus[0] && !postazioneStatus[1] && !postazioneStatus[2]){
        // Si en el frame anterior había alguna postazione activa
        if(touchwallStatus == true){
            // Activar salvapantallas general/fullscreen
            anim_salvap_fullscreen.play();
            // Parar salvapantallas de niña esperando interacción
            anim_salvap_peq.stop();
            alphaSalvap_peq = 0;
        }
        // El status general del touchwall es desactivado
        touchwallStatus = false;
        // Actualizar video salvapantallas fullscreen
        anim_salvap_fullscreen.update();
        // Alpha para video salvapantallas fullscreen
        if(alphaSalvap_full < 255){
            alphaSalvap_full += 10;
        }
    }
    // --- State 1 - "waiting touch" --- //
    else {
        // Si en el frame anterior no había ninguna postazione activa
        if(touchwallStatus == false){
            // Parar salvapantallas general/fullscreen
            anim_salvap_fullscreen.stop();
            alphaSalvap_full = 0;
            // Activar salvapantallas de niña esperando interacción
            anim_salvap_peq.play();
        }
        // El status general del touchwall es activado
        touchwallStatus = true;
        
        // Asignar video salvapantallas de niña esperando interacción
        // a una de las postaziones no activadas (waiting touch)
        if(postazioneStep[2] == "waiting touch"){
            salvap_peq_idx = 2;
        } else {
            if(postazioneStep[0] == "waiting touch"){
                salvap_peq_idx = 0;
            } else {
                if(postazioneStep[1] == "waiting touch"){
                    salvap_peq_idx = 1;
                }
            }
        }
    }
    
    // Recorremos las 3 postaziones
    for( int i=0; i<3; i++ ){
        
        // Manejar alfas
        if(alphaPostazione[i] < 255){
            alphaPostazione[i] += 10;
        }
        
        // --- State 1 - "waiting touch" --- //
        // Si se toca el botón touch de Postazione y el juego no está activo
        if(!postazioneStatus[i] && touchStatus[postElectIndex[i]] &&
           (postazioneStep[0] != "pre-game" && postazioneStep[1] != "pre-game" && postazioneStep[2] != "pre-game")){
            // Comenzar juego
            startPostazione(i);
            updateTimer(i);
        }
        
        // Si esta postazione está esperando a ser activada
        // y hay salvapantallas asignado a esta postazione
        if(postazioneStep[i] == "waiting touch" && salvap_peq_idx == i){
            // Actualizar video de salvapantallas pequeño (niña)
            anim_salvap_peq.update();
            // Alpha para video salvap peq (niña)
            if(alphaSalvap_peq < 255){
                alphaSalvap_peq += 10;
            }
        }
        
        // Para Postazione N=i - ACTIVA
        if(postazioneStatus[i]){
            
            // Recoger elapsed time
            float thisElapsedTime = ofGetElapsedTimef() - lastElapsedTime[i];
            
            // --- State 2 - "pre-game" --- //
            if(postazioneStep[i] == "pre-game"){
                // Actualizar animación pre-game
                anim_entra_vid[i].update();
                // frameIndex_p123[i] = (int)(thisElapsedTime * sequenceFPS) % anim_entra[i].size();
                // if(frameIndex_p123[i] == 0 && prevFrameIndex_p123[i]!=0){
                float finalVideoPos[3];
                finalVideoPos[0] = 0.82;
                finalVideoPos[1] = 0.5;
                finalVideoPos[2] = 0.75;
                if(anim_entra_vid[i].getPosition() > finalVideoPos[i]){
                // if(anim_entra_vid[i].getIsMovieDone()){
                    anim_entra_vid[i].stop();
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
    }
}

void game::draw(){
    // --- State 0 - "all waiting touch" --- //
    // Si no hay nada activado
    if(!touchwallStatus){
        ofPushStyle();
        ofSetColor(255, 255, 255, 255);
        // Video de salvapantallas fullscreen (general)
        anim_salvap_fullscreen.draw(postazione_0.x, postazione_0.y);
        ofPopStyle();
    }
    
    // Esto va antes para poder superponer sobre video de entrada
    for(int i=0; i<3; i++){
        // --- State 2 - "pre-game" --- //
        // Si estamos en pre-game (video de entrada de niño a postazione)
        if(postazioneStep[i] == "pre-game"){
            // Animación Postazione 1, 2, 3
            // Entrada a postazione -> video de nino_entra/pulsa_n
            ofPushStyle();
            ofSetColor(255, 255, 255, alphaPostazione[i]);
            anim_entra_vid[i].draw(postazione_0.x, postazione_0.y);
            ofPopStyle();
            // anim_entra[i][frameIndex_p123[i]].draw(postazione_0.x, postazione_0.y);
        }
    }
    
    // Recorremos las 3 postaziones
    for( int i=0; i<3; i++ ){
        // Para Postazione N=i - ACTIVA
        if(postazioneStatus[i]){
            ofPushStyle();
            ofSetColor(255, 255, 255, alphaPostazione[i]);
            
            // Recoger elapsed time
            float thisElapsedTime = ofGetElapsedTimef()-lastElapsedTime[i];
            
            // --- State 3 - "waiting answer" --- //
            if(postazioneStep[i] == "waiting for answer"){
                // Imagen pregunta
                img_preguntas[i][questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                
                // Imagen número de pregunta
                if(i==0 || i==2){
                    // Postazione 1, 3
                    num_pregunta_1_3[questionId[i]].draw(num_pregunta_pos[i].x, num_pregunta_pos[i].y);
                } else if (i==1) {
                    // Postazione 2
                    num_pregunta_2[questionId[i]].draw(num_pregunta_pos[i].x, num_pregunta_pos[i].y);
                }
                
                // Animación niño esperando / niño últimos segundos
                if(nino_esperando[i]){
                    if(i==0 || i==2) {
                        // Animación esperando Postazione 1 y 3
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_espera_1_3.size();
                        anim_espera_1_3[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                 anim_espera_1_3[frameIndex_nino[i]].getWidth()*0.88,
                                                                 anim_espera_1_3[frameIndex_nino[i]].getHeight()*0.88);
                    } else if(i==1) {
                        // Animación esperando Postazione 2
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_espera_2.size();
                        anim_espera_2[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                               anim_espera_2[frameIndex_nino[i]].getWidth()*0.88,
                                                               anim_espera_2[frameIndex_nino[i]].getHeight()*0.88);
                    }
                } else {
                    if(i==0 || i==2) {
                        // Animación últimos segundos Postazione 1 y 3
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_tiempo_1_3.size();
                        anim_tiempo_1_3[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                 anim_tiempo_1_3[frameIndex_nino[i]].getWidth()*0.88,
                                                                 anim_tiempo_1_3[frameIndex_nino[i]].getHeight()*0.88);
                    } else if(i==1) {
                        // Animación últimos segundos Postazione 2
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_tiempo_2.size();
                        anim_tiempo_2[frameIndex_nino[i]].draw(anim_pos[i].x + ajusteAnimTiempoPost2.x,
                                                               anim_pos[i].y + ajusteAnimTiempoPost2.y,
                                                               anim_tiempo_2[frameIndex_nino[i]].getWidth()*0.88,
                                                               anim_tiempo_2[frameIndex_nino[i]].getHeight()*0.88);
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
                ofSetColor(255, 255, 255, alphaPostazione[i]);
                img_arrow[i].draw(arrow_pos[i]);
                
                // Texto número tiempo restante
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
                    ofSetColor(0, alphaPostazione[i]);
                    tiempoRestante.drawString(ofToString((int)(maxAnswerTime-thisElapsedTime)),
                                              arrow_pos[i].x + marginW + extraX, arrow_pos[i].y + marginH + extraY);
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
                    // Imagen respuesta
                    img_respuestas[i][questionId[i]][imgAnswerId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    
                    // Imagen número de pregunta
                    if(i==0 || i==2){
                        // Postazione 1, 3
                        num_pregunta_1_3[questionId[i]].draw(num_pregunta_pos[i].x, num_pregunta_pos[i].y);
                    } else if (i==1) {
                        // Postazione 2
                        num_pregunta_2[questionId[i]].draw(num_pregunta_pos[i].x, num_pregunta_pos[i].y);
                    }
                }
                
                // Imagen fija cuando no hay respuesta - tiempo agotado (timeout)
                if(postazioneStep[i] == "showing timeout"){
                    // Imagen de pregunta
                    img_preguntas[i][questionId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                    
                    // Imagen de timeout
                    if(i==0 || i==2){
                        // Timeout - p. 1, 3
                        img_tiempoagotado_1_3.draw(num_pregunta_pos[i].x, num_pregunta_pos[i].y);
                    } else if (i==1) {
                        // Timeout - p. 2
                        img_tiempoagotado_2.draw(num_pregunta_pos[i].x, num_pregunta_pos[i].y);
                    }
                }
                
                // Animación niño respuesta correcta / incorrecta
                if(nino_correcto[i]){
                    if(i==0 || i==2) {
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_correcto_1_3.size();
                        anim_correcto_1_3[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                   anim_correcto_1_3[frameIndex_nino[i]].getWidth()*0.88,
                                                                   anim_correcto_1_3[frameIndex_nino[i]].getHeight()*0.88);
                    } else if(i==1) {
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_correcto_2.size();
                        anim_correcto_2[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                 anim_correcto_2[frameIndex_nino[i]].getWidth()*0.88,
                                                                 anim_correcto_2[frameIndex_nino[i]].getHeight()*0.88);
                    }
                } else {
                    if(i==0 || i==2) {
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_incorrecto_1_3.size();
                        anim_incorrecto_1_3[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                     anim_incorrecto_1_3[frameIndex_nino[i]].getWidth()*0.88,
                                                                     anim_incorrecto_1_3[frameIndex_nino[i]].getHeight()*0.88);
                    } else if(i==1) {
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_incorrecto_2.size();
                        anim_incorrecto_2[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                   anim_incorrecto_2[frameIndex_nino[i]].getWidth()*0.88,
                                                                   anim_incorrecto_2[frameIndex_nino[i]].getHeight()*0.88);
                    }
                }
            }
            
            // --- State 5 - "showing points" --- //
            else if(postazioneStep[i] == "showing points"){
                // Imagen Puntuacion
                if(i==0 || i==2){
                    // Postazione 1: Imagen para postazione 1 y 3
                    img_puntos_1_3[imgAnswerId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                } else if (i==1){
                    // Postazione 2: Imagen para postazione 2
                    img_puntos_2[imgAnswerId[i]].draw(postazionePos[i].x, postazionePos[i].y);
                }
                
                // Animación niño para puntuación aprobada / suspendida
                if(nino_correcto[i]){
                    if(i==0 || i==2) {
                        // Animación niño correcto para Postazione 1 y 3
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_correcto_1_3.size();
                        anim_correcto_1_3[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                   anim_correcto_1_3[frameIndex_nino[i]].getWidth()*0.88,
                                                                   anim_correcto_1_3[frameIndex_nino[i]].getHeight()*0.88);
                    } else if (i==1) {
                        // Animación niño correcto para Postazione 2
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_correcto_2.size();
                        anim_correcto_2[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                 anim_correcto_2[frameIndex_nino[i]].getWidth()*0.88,
                                                                 anim_correcto_2[frameIndex_nino[i]].getHeight()*0.88);
                    }
                } else {
                    if(i==0 || i==2) {
                        // Animación niño incorrecto para Postazione 1 y 3
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_incorrecto_1_3.size();
                        anim_incorrecto_1_3[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                     anim_incorrecto_1_3[frameIndex_nino[i]].getWidth()*0.88,
                                                                     anim_incorrecto_1_3[frameIndex_nino[i]].getHeight()*0.88);
                    } else if (i==1) {
                        // Animación niño incorrecto para Postazione 2
                        frameIndex_nino[i] = (int)(thisElapsedTime * sequenceFPS) % anim_incorrecto_2.size();
                        anim_incorrecto_2[frameIndex_nino[i]].draw(anim_pos[i].x, anim_pos[i].y,
                                                                   anim_incorrecto_2[frameIndex_nino[i]].getWidth()*0.88,
                                                                   anim_incorrecto_2[frameIndex_nino[i]].getHeight()*0.88);
                    }
                }
            }
            ofPopStyle();
        }
        // Para Postazione N=i - NO ACTIVA
        else {
            ofPushStyle();
            ofSetColor(255, 255, 255, alphaPostazione[i]);
            // --- State 1 - "waiting touch" --- //
            // Si otra Postazione está ACTIVA
            if(touchwallStatus){
                if(postazioneStep[i] == "waiting touch" && salvap_peq_idx == i){
                    // Salvapantallas Una Postazione
                    anim_salvap_peq.draw(salvap_peq_pos[i].x, salvap_peq_pos[i].y,
                                         anim_salvap_peq.getWidth()*0.24, anim_salvap_peq.getHeight()*0.24);
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
    // Reiniciamos video de entrada (nino_entra/pulsa_n) a postazione
    anim_entra_vid[postId].setFrame(0);
    anim_entra_vid[postId].play();
    saveInteraction();
}

void game::updateTimer(int postId){
    // Actualizar last elapsed time para la Postazione N=postId
    lastElapsedTime[postId] = ofGetElapsedTimef();
    frameIndex_nino[postId] = 0;
    if(postazioneStep[postId] != "showing answer" && postazioneStep[postId] != "showing timeout"){
        alphaPostazione[postId] = 0;
    }
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
    sequenceFPS = 18;
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
                anim_salvap_fullscreen.load("media/animacion/salvapantallas_fullscreen/screensave_general.mp4");
                anim_salvap_fullscreen.setLoopState(OF_LOOP_NORMAL);
                anim_salvap_fullscreen.play();
            }
                break;
                
                
            // STATE 1 - some postazione "waiting touch"
            case 1:
            {
                // Animación Salvapantallas Niña Una Postazione
                anim_salvap_peq.load("media/animacion/salvapantallas_postazione/screensave_peq.mp4");
                anim_salvap_peq.setLoopState(OF_LOOP_NORMAL);
            }
                break;
                
                
            // STATE 2 - "pre-game"
            case 2:
            {
                // POSTAZIONE 1, 2, 3 (different animations)
                for(int i=0; i<3; i++){
//                    d.listDir("media/animacion/nino_entra/postazione_" + ofToString(i+1));
//                    for(int j=0; j<d.size(); j++){
//                        ofFile file(ofToDataPath(d.getPath(j)));
//                        if(file.getExtension() == "png"){
//                            anim_entra[i].push_back(ofImage());
//                            anim_entra[i][j].load(d.getPath(j));
//                        }
//                    }
                    // Video de entrada de niño a postazione
                    string filePath = "media/animacion/nino_entra/postazione_" + ofToString(i+1) + "/pulsa_n" + ofToString(i+1) + ".mp4";
                    anim_entra_vid[i].load(filePath);
                    anim_entra_vid[i].setLoopState(OF_LOOP_NONE);
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

void game::saveInteraction(){
    // Recoger fecha actual
    int month = ofGetMonth();
    string month_str;
    if(month<10){
        month_str = "0" + ofToString(month);
    } else {
        month_str = ofToString(month);
    }
    int day = ofGetDay();
    string day_str;
    if(day<10){
        day_str = "0" + ofToString(day);
    } else {
        day_str = ofToString(day);
    }
    string date = ofToString(ofGetYear()) + "-" + month_str + "-" + day_str;
    
    // Crear carpeta csvInteractionCounter si no existe
    string folder = "csvInteractionCounter/";
    ofDirectory dir(folder);
    if(!dir.exists()){
        dir.create(true);
    }
    
    // Abrir CSV
    string csvPath = folder + "interactionData.csv";
    ofFile file(ofToDataPath(csvPath));
    if(file.exists()){
        csvInteraction.load(csvPath);
    } else {
        csvInteraction.createFile(csvPath);
        csvInteraction.addRow();
        csvInteraction[0].addString("Date");
        csvInteraction[0].addString("Number of Interactions");
    }
    bool newDate = true;
    if(csvInteraction.getNumRows()>1){
        // Recorremos todas las filas del documento
        for(int i=1; i<csvInteraction.getNumRows(); i++){
            // Si la fecha actual ya existe en el csv
            if(csvInteraction[i][0] == date){
                // Si es la primera interacción de la ejecución
                if(numOfInteractionsToday==0){
                    // Recoger número de interacciones existentes para este día desde csv
                    numOfInteractionsToday = ofToInt(csvInteraction[i][1]) + 1;
                } else {
                    // Sumar interacción
                    numOfInteractionsToday += 1;
                }
                // Escribir número de interacción en csv
                csvInteraction[i][1] = ofToString(numOfInteractionsToday);
                // Ya encontramos la fecha - no hay que seguir el for
                newDate = false;
                break;
            }
        }
    }
    
    // Si la fecha no existe en el csv creamos nueva fila
    if(newDate){
        numOfInteractionsToday = 1;
        csvInteraction.addRow();
        // Escribir nueva fecha y número de interacción en csv
        csvInteraction[csvInteraction.getNumRows()-1].addString(date);
        csvInteraction[csvInteraction.getNumRows()-1].addInt(numOfInteractionsToday);
    }
    // Guardar Csv
    csvInteraction.save(csvPath);
}