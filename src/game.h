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
    
        // Debug
        bool is_debugging;
    
        // Directorio Imágenes
        ofDirectory dir;
    
        // Texto en pantalla
        ofTrueTypeFont timeFont;
        ofTrueTypeFont pointsFont;
    
        // WALL & PLACA
        // Bare Conductive
        int nElectrodes;
        // Estado del electrodo (touched/not touched)
        vector<bool> touchStatus;
        // Indice del electrodo asociado a cada uno
        // de los botones de Play Postazione
        int postElectIndex[3];
        // Indice del electrodo asociado a cada uno
        // de los botones de Opciones ABC
        int optionElectIndex[3][3];

        // POSTAZIONE 1, 2, 3
        // Estado del touchwall (any postazione is active?)
        bool touchwallStatus;
        // Estado de la postazione (playing/stopped)
        bool postazioneStatus[3];
        // Pasos de cada pregunta en cada postazione
        string postazioneStep[3];
        // Puntos para cada postazione
        int postazionePoints[3];
        // Posición en pantalla de cada una de las postaziones
        ofVec2f postazionePos[3];
        // Posición de postazione 0 a la izquierda (información)
        ofVec2f postazione_0;
        // Indices de pregunta y respuesta para cada postazione
        int questionId[3];
        int imgAnswerId[3];
        // Alpha de imágenes [FIJO DE MOMENTO]
        int alphaPostazione[3];

        // TODO: Pasar a XML
        // Respuestas correctas (indice)
        int postCorrectAnswer[3][5];
        // Tiempo máximo para responder
        float maxAnswerTime = 15;
        // Últimos segundos para cada pregunta
        // (Activa otra animación)
        float lastSecondsAmount = 5;
        // Tiempo entre preguntas
        float timeToNextQuestion = 5;
        // Tiempo para la puntuación
        float timeToEnjoyPoints = 10;
        // Puntos mínimos para aprobar el juego
        int pointsToPass = 60;

        // ANIMACIONES
        // FPS de secuencia
        int sequenceFPS;
        // Índice de frame para las animaciones del niño
        int frameIndex_kid[3];
        // Índice de frame para la Postazione 0
        int frameIndex_p0;
        // Índice de frame para la Postazione 1, 2, 3
        int frameIndex_p123[3];
        int prevFrameIndex_p123[3]; // frame previo
        // Último elapsed time guardado en postaziones
        float lastElapsedTime[3]; //p. 1, 2, 3

    
        // ------------- STATES ------------- //
        // --- State 0 - "all waiting touch" --- //
        // Animación Postazione 0
        vector<ofImage> a_pos0_st0;
    
        // --- State 1 - "waiting touch" --- //
        // Animación Postazione 0
        vector<ofImage> a_pos0_st1;
        // Animación Postazione 1, 2, 3
        vector<ofImage> a_pos123_st1[3];
    
        // --- State 2 - "pre-game" --- //
        // Animación Postazione 1, 2, 3
        vector<ofImage> a_pos123_st2[3];
    
        // --- State 3 - "waiting answer" --- //
        // Animación Postazione 0
        vector<ofImage> a_pos0_st3;
        // Imágenes para las preguntas
        vector<ofImage> i_pos123_st3[3]; // postazione 1, 2, 3
        // Imagen de flecha de tiempo (animación por rotación)
        ofImage i_arrow[3]; // p. 1, 2, 3
        // Posición imagen flecha tiempo
        ofVec2f arrow_pos[3]; // p. 1, 2, 3
        // Animación niño esperando a respuesta
        vector<ofImage> a_kid_wait[3]; // p. 1, 2, 3
        // Animación niño esperando (últimos segundos)
        vector<ofImage> a_kid_time[3]; // p. 1, 2, 3
        // Niño esperando (true) o niño en últimos segundos (false)
        bool kid_wait_st3[3]; // p. 1, 2, 3


        // --- State 4 - "showing answer or timeout" --- //
        // Animación Postazione 0
        vector<ofImage> a_pos0_st4;
        // Imágenes para respuesta elegida o imagen timeout
        vector<ofImage> i_pos123_st4[3][4]; // p. 1, 2, 3
        // Animación niño (respuesta correcta)
        vector<ofImage> a_kid_correct_st4[3]; // p. 1, 2, 3
        // Animación niño (respuesta incorrecta)
        vector<ofImage> a_kid_wrong_st4[3]; // p. 1, 2, 3
        // Niño respuesta correcta (true) o incorrecta (false)
        bool kid_correct_st4[3]; // p. 1, 2, 3
    
        // --- State 5 - "showing points" --- //
        // Animación Postazione 0
        vector<ofImage> a_pos0_st5;
        // Imagen de libro o copa según puntuación
        ofImage i_points_img[3][2]; // p. 1, 2, 3
        // Animación niño puntuación positiva (aprobado)
        vector<ofImage> a_kid_correct_st5[3];
        // Animación niño puntuación negativa (suspendido)
        vector<ofImage> a_kid_wrong_st5[3];
        // Niño puntuación aprobada (true) o suspendida (false)
        bool kid_correct_st5[3];
        // ------------- STATES ------------- //
};