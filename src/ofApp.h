#pragma once

#include "ofMain.h"

#include "touchBoard.h"
#include "gui.h"
#include "settings.h"
#include "game.h"

#define N_ELECT 12

class ofApp : public ofBaseApp{
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // Window
    int WIDTH, HEIGHT;
    bool fullscreen;
    
    // Settings
    int appFPS;
    settings gameSettings;
    
    // Touch Board
    bool keyboardDebug;
    vector<bool> touchStatus_debug;
    touchBoard touch;
    bool bareConductive;
    
    // Game
    game game;
    
    // GUI
    gui gui;
};
