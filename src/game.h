//
//  game.hpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 04/08/17.
//
//

#include "ofMain.h"
#include "settings.h"

class game {
    
    public:
        void setup(int nElect, settings gameSettings);
        void update(vector<bool> touchStatus);
        void draw();
    
        // Cargar todas las imagenes
        void loadAllImages();
    
        // Utilizar para cargar settings de xml
        void loadXmlSettings(settings gameSettings);
        // Utilizar si no queremos utilizar los settings de xml
        void loadXmlValuesManual();
    
        void startPostazione(int postId);
        void updateTimer(int postId);
    
        // Debug
        bool is_debugging;
    
        // Directorio Imágenes
        ofDirectory dir;
    
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
        // ofVec2f postazionePos[3];
        // Posición en pantalla de cada una de las postaziones
        ofVec2f postazionePos[3];
        // Posición de postazione 0 a la izquierda (información)
        ofVec2f postazione_0;
        // Ajuste de transparencias de pregunta para Postazione 3
        // (no para el niño)
        ofVec2f ajustePreguntaPost3;
        // Indices de pregunta y respuesta para cada postazione
        int questionId[3];
        int imgAnswerId[3];
        // Alpha de imágenes [FIJO DE MOMENTO]
        int alphaPostazione[3];

        // TODO: Pasar a XML
        // Respuestas correctas (indice)
        int postCorrectAnswer[3][5];
        // Tiempo máximo para responder
        float maxAnswerTime;
        // Últimos segundos para cada pregunta
        // (Activa otra animación)
        float lastSecondsAmount;
        // Tiempo entre preguntas
        float timeToNextQuestion;
        // Tiempo para la puntuación
        float timeToEnjoyPoints;
        // Puntos mínimos para aprobar el juego
        int pointsToPass;
        // Número de respuestas por pregunta
        int numAnswerPerQuestion[3][5];
        // Enseñar tiempo restante activado/desactivado
        bool tiempoRestActivo;

        // ANIMACIONES
        // FPS de secuencia
        int sequenceFPS;
        // Índice de frame para las animaciones del niño
        int frameIndex_nino[3];
        // Índice de frame para la Postazione 0
        int frameIndex_salvap_full;
        // Índice de frame para la Postazione 1, 2, 3
        int frameIndex_p123[3];
        int prevFrameIndex_p123[3]; // frame previo
        // Último elapsed time guardado en postaziones
        float lastElapsedTime[3]; //p. 1, 2, 3

    
        // ------------- STATES ------------- //
        // --- VARIOS STATES --- //
        // Animación niño correcto
        vector<ofImage> anim_correcto_1_3;
        vector<ofImage> anim_correcto_2;
        // Animación niño incorrecto
        vector<ofImage> anim_incorrecto_1_3;
        vector<ofImage> anim_incorrecto_2;
        // Animacion niño correcto (true) o incorrecta (false)
        bool nino_correcto[3];
    
        // --- State 0 - "all waiting touch" --- //
        // Animación Postazione 0
        vector<ofImage> anim_salvap_fullscreen;
    
        // --- State 1 - "waiting touch" --- //
        // Animación Salvapantalla Una Postazione
        vector<ofImage> anim_salvap_1_3;
        vector<ofImage> anim_salvap_2;
    
        // --- State 2 - "pre-game" --- //
        // Animación Niño entra a Postazione 1, 2, 3
        vector<ofImage> anim_entra[3];
    
        // --- State 3 - "waiting answer" --- //
        // Imágenes para las preguntas
        ofImage img_preguntas[3][5]; // postazione 1, 2, 3
        // Animación niño esperando a respuesta
        vector<ofImage> anim_espera_1_3; // p. 1 y 3
        vector<ofImage> anim_espera_2; // p. 2
        // Animación niño (últimos segundos)
        vector<ofImage> anim_tiempo_1_3; // p. 1 y 3
        vector<ofImage> anim_tiempo_2; // p. 2
        // Imagen de flecha de tiempo (animación por rotación)
        ofImage img_arrow[3]; // p. 1, 2, 3
        // Posición imagen flecha tiempo
        ofVec2f arrow_pos[3]; // p. 1, 2, 3
        // Niño esperando (true) o niño en últimos segundos (false)
        bool nino_esperando[3]; // p. 1, 2, 3
        // Imagen con número de pregunta
        ofImage num_pregunta_1_3[5];
        ofImage num_pregunta_2[5];
        // Texto de tiempo restante en pantalla
        ofTrueTypeFont tiempoRestante;
    
        // --- State 4 - "showing answer or timeout" --- //
        // Imágenes para respuesta elegida o imagen timeout
        ofImage img_respuestas[3][5][3];
        ofImage img_tiempoagotado_1_3;
        ofImage img_tiempoagotado_2;
    
        // --- State 5 - "showing points" --- //
        // Imagen Puntuacion Final
        ofImage img_puntos_1_3[6]; // p. 1 y 3
        ofImage img_puntos_2[6]; // p. 2
        // ------------- STATES ------------- //
};