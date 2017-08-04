//
//  game.hpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 04/08/17.
//
//

#include "ofMain.h"

class game {
    
    public:
        void setup(int nElect);
        void update(vector<bool> touchStatus);
        void draw();
    
        void loadButtons();
        void startPostazione(int postId);
        void updateTimer(int postId);
    
        // Bare Conductive
        int nElectrodes;
    
        // Directorio Imágenes
        ofDirectory dir;
    
        // Postazione
        ofImage postazione[3][6][5];
        string postImgName[3][6][5];
        int postCorrectAnswer[3][5];
    
        //
        int questionId[3];
        int imgId[3];
        int alphaPostazione[3];
        ofVec2f postazionePos[3];
    
        // Estado del electrodo (touched/not touched)
        vector<bool> touchStatus;
        // Estado del nivel de juego (playing/stopped)
        bool postazioneStatus[3];

        // Indice del electrodo asociado a cada uno
        // de los botones de Play Postazione
        int postElectIndex[3];
        // Indice del electrodo asociado a cada uno
        // de los botones de Opciones ABC
        int optionElectIndex[3][3];
    
        // Pasos de cada pregunta
        string postazioneStep[3];
        int postazionePoints[3];
    
        // Tiempo máximo para responder
        float maxAnswerTime = 15;
        // Tiempo entre preguntas
        float timeToNextQuestion = 1;
        // Tiempo al final
        float timeToEnjoyPoints = 20;
        // Último elapsed time guardado en postaziones
        float lastElapsedTime[3];
    
        ofTrueTypeFont pointsFont;
};