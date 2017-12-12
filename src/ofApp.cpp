#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Window
    // ofBackground(236, 238, 240, 255);
    ofBackground(0);
    ofSetWindowTitle("Touch Wall v0.1");
    WIDTH=1080;
    HEIGHT=1920;
    fullscreen = true;
    ofSetFullscreen(fullscreen);
    
    // Settings
    appFPS = 60;
    ofSetFrameRate(appFPS);
    gameSettings.setup();
    
    // Touch
    bareConductive = false;
    if(bareConductive){
        touch.setup(N_ELECT, gameSettings.touchDeviceId);
    }
    keyboardDebug = true;
    if(keyboardDebug){
        // Initialize Touch Debug Status
        for(int i=0; i<12; i++){
            touchStatus_debug.push_back(false);
        }
    }
    
    // Game
    game.setup(N_ELECT, gameSettings);
    
    // GUI
    gui.setup(N_ELECT, gameSettings.guiActive);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(bareConductive){
        touch.update();
    }
    
    if(!keyboardDebug){
        game.update(touch.touchStatus);
        gui.update(touch.touchStatus);
    } else {
        game.update(touchStatus_debug);
        gui.update(touchStatus_debug);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    gui.draw(255);

    game.draw();
    
    // gui.draw(90);
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
    
    if(keyboardDebug){
        // Touch Status Debug
        if(key == '1'){
            // Postazione 1 - Play
            touchStatus_debug[gameSettings.postazioneElectIndex[0]] = true;
        }
        if(key == '2'){
            // Postazione 2 - Play
            touchStatus_debug[gameSettings.postazioneElectIndex[1]] = true;
        }
        if(key == '3'){
            // Postazione 3 - Play
            touchStatus_debug[gameSettings.postazioneElectIndex[2]] = true;
        }
        if(key == 'q' || key == 'Q'){
            // Postazione 1 - A
            touchStatus_debug[gameSettings.abcElectIndex[0][0]] = true;
        }
        if(key == 'w' || key == 'W'){
            // Postazione 1 - B
            touchStatus_debug[gameSettings.abcElectIndex[0][1]] = true;
        }
        if(key == 'e' || key == 'E'){
            // Postazione 1 - C
            touchStatus_debug[gameSettings.abcElectIndex[0][2]] = true;
        }
        if(key == 'a' || key == 'A'){
            // Postazione 2 - A
            touchStatus_debug[gameSettings.abcElectIndex[1][0]] = true;
        }
        if(key == 's' || key == 'S'){
            // Postazione 2 - B
            touchStatus_debug[gameSettings.abcElectIndex[1][1]] = true;
        }
        if(key == 'd' || key == 'D'){
            // Postazione 2 - C
            touchStatus_debug[gameSettings.abcElectIndex[1][2]] = true;
        }
        if(key == 'z' || key == 'Z'){
            // Postazione 3 - A
            touchStatus_debug[gameSettings.abcElectIndex[2][0]] = true;
        }
        if(key == 'x' || key == 'X'){
            // Postazione 3 - B
            touchStatus_debug[gameSettings.abcElectIndex[2][1]] = true;
        }
        if(key == 'c' || key == 'c'){
            // Postazione 3 - C
            touchStatus_debug[gameSettings.abcElectIndex[2][2]] = true;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(keyboardDebug){
		// Touch Status Debug
		if (key == '1') {
			// Postazione 1 - Play
			touchStatus_debug[gameSettings.postazioneElectIndex[0]] = false;
		}
		if (key == '2') {
			// Postazione 2 - Play
			touchStatus_debug[gameSettings.postazioneElectIndex[1]] = false;
		}
		if (key == '3') {
			// Postazione 3 - Play
			touchStatus_debug[gameSettings.postazioneElectIndex[2]] = false;
		}
		if (key == 'q' || key == 'Q') {
			// Postazione 1 - A
			touchStatus_debug[gameSettings.abcElectIndex[0][0]] = false;
		}
		if (key == 'w' || key == 'W') {
			// Postazione 1 - B
			touchStatus_debug[gameSettings.abcElectIndex[0][1]] = false;
		}
		if (key == 'e' || key == 'E') {
			// Postazione 1 - C
			touchStatus_debug[gameSettings.abcElectIndex[0][2]] = false;
		}
		if (key == 'a' || key == 'A') {
			// Postazione 2 - A
			touchStatus_debug[gameSettings.abcElectIndex[1][0]] = false;
		}
		if (key == 's' || key == 'S') {
			// Postazione 2 - B
			touchStatus_debug[gameSettings.abcElectIndex[1][1]] = false;
		}
		if (key == 'd' || key == 'D') {
			// Postazione 2 - C
			touchStatus_debug[gameSettings.abcElectIndex[1][2]] = false;
		}
		if (key == 'z' || key == 'Z') {
			// Postazione 3 - A
			touchStatus_debug[gameSettings.abcElectIndex[2][0]] = false;
		}
		if (key == 'x' || key == 'X') {
			// Postazione 3 - B
			touchStatus_debug[gameSettings.abcElectIndex[2][1]] = false;
		}
		if (key == 'c' || key == 'c') {
			// Postazione 3 - C
			touchStatus_debug[gameSettings.abcElectIndex[2][2]] = false;
		}
    }
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
