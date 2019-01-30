#pragma once
#include "ofMain.h"
#include "Game.hpp"

/*--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
 BACTERIA
 
 Bacteria is a game based on the Cellular Automata concept, more specifically on the Conway's Game of Life.
 The name comes from the Bacterial Cell Growth process, which is similar to how the Cellular Automata works.
 
 The player should kill all the bacterial cells in the environment, to do it he might shoot cells against the enemies. But these cells reproduce in a very efficient way, so we should shot with a logic, following the cell reproduction's rules.
 
 The rules of the bacterial cells are very simple:
     - Each alive cell with one or no neighbors dies, as if by solitude.
     - Each alive cell with four or more neighbors dies, as if by overpopulation.
     - Each alive cell with two or three neighbors survives.
     - Each dead cell with three neighbors becomes populated.
 
 
 SOME IMPORTANT DESIGN CHOICES:
 -the game requires 2 addons: ofxGui and ofxMaxim
 -for convenience, the game's grid is builded on x and y axis instead of on x and z. Thanks to this, I work only with ofPoint(a,b), instead of ofPoint(a, 1, b).
 -the center coordinates are in the point (0,0,0), because in the game an ofTransition is used.
 

 OFAPP

 The ofApp Class is an openFrameworks default class.
 In this case it handles mainly the cam and the lights.
 
 The methods are:
 
 -setup() => allows to initialize some OF settings, the game, the cam and the light
 -update() => allows to update the game
 -draw() => allows to draw the game with the cam and lights POV and the GUI with the standard 2d POV
 -audioOut => allows to pass the event to the game class
 
 --**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--***/


class ofApp : public ofBaseApp{
    private:
        Game game;
        ofColor bgColor;
        ofCamera cam;
        ofLight light;

        const int sampleRate = 44100;   // in Hz
        /* the size of the buffer, the number of floating-point values in the input array.
         + buffersize = - calls to the audio hardware, but + delay
        */
        const int bufferSize = 512;
        const int frameRate = 60;       // fps
    
	public:
		void setup();
		void update();
		void draw();
        void audioOut(float * output, int bufferSize, int nChannels);
};
