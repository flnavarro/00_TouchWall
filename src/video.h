//
//  video.hpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 27/07/17.
//
//

#include "ofMain.h"

#define LOAD_TYPE 0

class video {
    
    public:
        void setup(int nElect,
                   string videoType, string videoLoadingMode,
                   vector<ofPoint> videoPosition,
                   vector <float> videoWidth,
                   vector <string> videoElectrode);
        void update(vector<bool> touchStatus);
        void draw();
    
        void loadVideo(int videoId);
        void playVideo(int videoId);
        void updateVideoSettings(vector<ofPoint> newVideoPosition);
    
        // Bare Conductive
        int nElectrodes;
    
        // Directorio Videos
        ofDirectory dir;
        // Tipo de vídeo
        string vidType;
        // Tipo de carga
        string loadingMode;
        // Posición de video en pantalla
        vector<ofPoint> vidPos;
        // Tamaño del archivo de video
        vector<ofVec2f> vidSize;
        // Anchura de video en Touchwall
        vector<float> vidWidth;
        // Tamaño del video en TouchWall
        vector<ofVec2f> vidSizeInWall;
    
        // Electrodo asociado a cada video
        vector<string> vidElectrode;
    
        // Formato PNG
        vector<vector<ofImage>> pngVideo;
        float sequenceFPS;
        bool  bFrameIndependent;
        vector<int> frameIndex;
        vector<float> seqElapsedTime;
    
        // Formato Video
        vector<ofVideoPlayer> vidVideo;
    
        // Estado del electrodo (touched/not touched)
        vector<bool> touchStatus;
        // Estado del video (playing/stopped)
        vector<bool> videoStatus;
};
