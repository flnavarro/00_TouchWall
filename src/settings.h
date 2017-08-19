//
//  settings.hpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 27/07/17.
//
//
#pragma once

#include "ofMain.h"

class settings {

    public:
        void setup();
    
        // Touchwall Settings
        ofXml touchWallSettings;
        // TouchBoard ID
        int touchDeviceId;
        // Electrodos de TouchBoard asociados al juego
        int postazioneElectIndex[3];
        int abcElectIndex[3][3];

        // Game Settings Xml
        ofXml gameSettings;
        // GUI Activo al abrir?
        bool guiActive;
        // Tiempo restante (números) activo?
        bool showRemainingTime;
        // Sequence FPS para animaciones
        int sequenceFPS;
        // Tiempo: Máximo para responder preguntas
        float maxAnswerTime;
        // Tiempo: Últimos segundos para responder
        float lastSecondsTime;
        // Tiempo: Para pasar a la siguiente pregunta
        float toNextQuestionTime;
        // Tiempo: Para mostrar puntos
        float pointsTime;
        // Puntos para aprobar/animación "feliz"
        int pointsToPass;
        // Respuestas Correctas
        int postCorrectAnswer[3][5];
        // Número de respuestas por pregunta
        int numAnswerPerQuestion[3][5];

};
