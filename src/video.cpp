//
//  video.cpp
//  00_TouchWall
//
//  Created by Felipe L. Navarro on 27/07/17.
//
//

#include "video.h"

void video::setup(int nElect,
                  string videoType, string videoLoadingMode,
                  vector<ofPoint> videoPosition, vector <float> videoWidth,
                  vector <string> videoElectrode){
    
    nElectrodes = nElect;
    vidType = videoType;
    loadingMode = videoLoadingMode;
    vidPos = videoPosition;
    vidWidth = videoWidth;
    vidElectrode = videoElectrode;
    
    // Inicializar settings secuencia
    sequenceFPS = 24;
    bFrameIndependent = true;
    
    // Inicializar
    string vidFolder;
    if(vidType == "PNG"){
        vidFolder = "videosPNG";
    } else if (vidType == "VID"){
        vidFolder = "videos";
    }
    dir.listDir(vidFolder);
    for(int i=0; i<nElectrodes; i++){
        // Inicializar Size
        vidSize.push_back(ofVec2f(0,0));
        vidSizeInWall.push_back(ofVec2f(0,0));
        if(vidType == "PNG"){
            // Frame index de cada secuencia
            frameIndex.push_back(0);
            seqElapsedTime.push_back(0);
            // Inicializar array secuencias
            vector<ofImage> emptySequence;
            for(int i=0; i<5; i++){
                emptySequence.push_back(ofImage());
            }
            pngVideo.push_back(emptySequence);
            // Cargar secuencias si el modo es INIT
            if(loadingMode == "INIT"){
                loadVideo(i);
            }
        } else if(vidType == "VID"){
            // Inicializar array videos
            vidVideo.push_back(ofVideoPlayer());
            // Cargar videos si el modo es INIT
            if(loadingMode == "INIT"){
                loadVideo(i);
            }
        }
        // Inicializar estados de electrodo y videos
        touchStatus.push_back(false);
        videoStatus.push_back(false);
    }
}

void video::update(vector<bool> touchStatus){
    for(int i=0; i<nElectrodes; i++){
        if(touchStatus[i] && !videoStatus[i]){
            // Play video:
            // Si hay touch en esta posición y
            // El video correspondiente no está ya activo
            playVideo(i);
        }
        
        // Si el video está activo
        if(videoStatus[i]){
            // Si es secuencia de png
            if(vidType == "PNG"){
                // Calcular el frameIndex de esta secuencia
                float elapsedPlayTime = ofGetElapsedTimef() - seqElapsedTime[i];
                int prevFrameIndex = frameIndex[i];
                frameIndex[i] = (int)(elapsedPlayTime * sequenceFPS) % pngVideo[i].size();
                if(frameIndex[i]<prevFrameIndex){
                    // Si vuelve al primer frame, false
                    videoStatus[i] = false;
                    // Liberar array si el modo es request
                    if(loadingMode == "REQUEST"){
                        for(int j=0; j<pngVideo[i].size(); j++){
                            pngVideo[i][j].clear();
                        }
                    }
                }
            // Si es formato video
            } else if (vidType == "VID"){
                vidVideo[i].update();
                if(vidVideo[i].getIsMovieDone()){
                    // Al terminar el video volvemos a false
                    videoStatus[i] = false;
                    // Cerrar video si el modo es request
                    if(loadingMode == "REQUEST"){
                        vidVideo[i].closeMovie();
                    }
                }
            }
        }
    }
}

void video::draw(){
    for(int i=0; i<nElectrodes; i++){
        if(videoStatus[i]){
            ofSetColor(255,255,255,255);
            if(vidType == "PNG"){
                // Draw frame de la secuencia
                pngVideo[i][frameIndex[i]].draw(vidPos[i].x, vidPos[i].y,
                                                vidSizeInWall[i].x, vidSizeInWall[i].y);
            } else if(vidType == "VID"){
                // Draw frame de video
                vidVideo[i].draw(vidPos[i].x, vidPos[i].y,
                                 vidSizeInWall[i].x, vidSizeInWall[i].y);
            }
        }
    }
}

void video::loadVideo(int videoId){
    string dirPath = dir.getPath(videoId);
    ofVec2f size;
    if (vidType == "PNG"){
        // Cargar secuencias de pngs
        ofDirectory seqDir;
        seqDir.listDir(dirPath);
        vector<ofImage> imgSequence;
        for(int j=0; j<seqDir.size(); j++){
            string imgPath = seqDir.getPath(j);
            imgSequence.push_back(ofImage());
            imgSequence.back().load(imgPath);
        }
        pngVideo[videoId] = imgSequence;
        // Tamaño de los frames de la secuencia
        size.x = pngVideo[videoId][0].getWidth();
        size.y = pngVideo[videoId][0].getHeight();
    } else if (vidType == "VID"){
        // Cargar videos
        vidVideo[videoId].load(dirPath);
        // Settings video: sin loop y sin volumen
        vidVideo[videoId].setLoopState(OF_LOOP_NONE);
        vidVideo[videoId].setVolume(0);
        // Tamaño de video
        size.x = vidVideo[videoId].getWidth();
        size.y = vidVideo[videoId].getHeight();
    }
    // Tamaño de video
    vidSize[videoId] = size;
    // Calcular nuevo tamaño para touchwall
    ofVec2f sizeInWall;
    float resizeIndex = (float)vidWidth[videoId]/(float)vidSize[videoId].x;
    sizeInWall.x = vidSize[videoId].x * resizeIndex;
    sizeInWall.y = vidSize[videoId].y * resizeIndex;
    vidSizeInWall[videoId] = sizeInWall;
}

void video::playVideo(int videoId){
    // Cargar video si el modo es request
    if(loadingMode == "REQUEST"){
        loadVideo(videoId);
    }
    if(vidType == "PNG"){
        // Guardar el elapsed time del momento Play
        seqElapsedTime[videoId] = ofGetElapsedTimef();
    } else if (vidType == "VID"){
        // Play video
        vidVideo[videoId].play();
    }
    videoStatus[videoId] = true;
}

void video::updateVideoSettings(vector<ofPoint> newVideoPosition){
    vidPos = newVideoPosition;
}
