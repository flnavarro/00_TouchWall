#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // Window
    ofBackground(0);
    ofSetWindowTitle("Touch Wall v0.1");
    WIDTH=1080;
    HEIGHT=1920;
    fullscreen = true;
    ofSetFullscreen(fullscreen);
    
    // Settings
    appFPS = 60;
    ofSetFrameRate(appFPS);
    settings.setup();
    
    // Touch
    touch.setup(N_ELECT, settings.touchDeviceId);
    
    // Game
    game.setup(N_ELECT);
    
    // GUI
    gui.setup(N_ELECT, settings.guiActive, settings.videoPosition);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    touch.update();
    
    gui.update(touch.touchStatus);
    // if( gui.updateVideoSettings ){
        // settings.updateVideoSettings(gui.guiPos);
    // }
    
    game.update(touch.touchStatus);
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gui.draw();

    game.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f' || key == 'F'){
        fullscreen = !fullscreen;
        ofSetFullscreen(fullscreen);
    }
    if(key == 'g' || key == 'G'){
        gui.changeStatus();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
