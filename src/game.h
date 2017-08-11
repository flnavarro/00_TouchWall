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
    
        void loadAllImages();
    
        void loadButtons();
        void startPostazione(int postId);
        void updateTimer(int postId);
    
        // Bare Conductive
        int nElectrodes;
    
        // Directorio Imágenes
        ofDirectory dir;
    
        // Postazione
        int postCorrectAnswer[3][5];
    
        //
        int questionId[3];
        int imgAnswerId[3];
        int alphaPostazione[3];
        ofVec2f postazionePos[3];
    
        // Estado del electrodo (touched/not touched)
        vector<bool> touchStatus;
        // Estado de la postazione (playing/stopped)
        bool postazioneStatus[3];
        // Estado del touchwall (any postazione is active?)
        bool touchwallStatus;

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
        float timeToNextQuestion = 5;
        // Tiempo al final
        float timeToEnjoyPoints = 20;
        // Último elapsed time guardado en postaziones
        float lastElapsedTime[3];
        float lastSecondsAmount = 5;
    
        ofTrueTypeFont timeFont;
        ofTrueTypeFont pointsFont;
    
        int frameIndex_kid[3];
    
        // Anim index
        int sequenceFPS;
        int frameIndex_p0;
        int frameIndex_p123[3];
        int prevFrameIndex_p123[3];
    
        ofVec2f postazione_0;
    
        // State 0 - "all waiting touch"
        vector<ofImage> a_pos0_st0;
    
        // State 1 - "waiting touch"
        vector<ofImage> a_pos0_st1;
        vector<ofImage> a_pos123_st1[3];
    
        // State 2 - "pre-game"
        vector<ofImage> a_pos123_st2[3];
        //vector<ofImage> a_pos2_st2;
        //vector<ofImage> a_pos3_st2;
    
        // State 3 - "waiting answer"
        vector<ofImage> a_pos0_st3;
        ofImage i_arrow[3];
        ofVec2f arrow_pos[3];
        vector<ofImage> a_kid_wait[3];
        vector<ofImage> a_kid_time[3];
        vector<ofImage> i_pos123_st3[3];
        //ofImage pos2_st3[5];
        //ofImage pos3_st3[5];

        // State 4 - "showing result"
        vector<ofImage> a_kid_correct_st4[3];
        vector<ofImage> a_kid_wrong_st4[3];
        vector<ofImage> a_pos0_st4;
        vector<ofImage> i_pos123_st4[3][4];
        ofImage pos2_st4[5][4];
        ofImage pos3_st4[5][4];
    
        // State 5 - "showing points"
        vector<ofImage> a_kid_correct_st5[3];
        vector<ofImage> a_kid_wrong_st5[3];
        vector<ofImage> a_pos0_st5;
        ofImage pointsImg[2];
    
        bool kid_wait_st3[3];
        bool kid_correct_st4[3];
        bool kid_correct_st5[3];
    
        bool is_debugging;
};