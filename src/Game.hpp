#pragma once
#include "ofMain.h"
#include "Cell.hpp"
#include "Environment.hpp"
#include "Soundtrack.hpp"
#include "GUI.hpp"


/*--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**

 GAME
 The Game class handles the game's logic and links it to the grid (environment), the soundtrack and the GUI.
 
 The methods are:
 
 -setup() => it initializes the game's parameters
 -update() => it contains the game's logic
 -draw() => it draws the game (with the rotations) in a 3D world
 -drawGUI() => it draws the GUI if the game is paused (this is in a 2D world)
 -keyPressed() => it handles all the commands for the player + the pause button
 -nextLevel() => it allows to go to the next level
 -repeatLevel() => it allows to repeats the current level
 -loadLevels() => it loads levels from the levels.txt file in the bin/data folder
 -levelsParser() => it parses the txt levels file, and returns a vector of levels
 -levelChecker() => it checks for some common errors in the levels file
 -getGameSize() => it returns the game's size (it considers the grid's size and the cell's size)
 -exit() => it allows to close the audio stream
 -audioOut() => it allows to pass the audio data to the Soundtrack class

 --**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--***/


class Game{
    
    private:
        Environment environment;
        Soundtrack soundtrack;
        GUI gui;
        string logFileName;
    
        vector<vector<vector<Cell>>> levels;        //vector of matrices (game's grids)
        vector<int> levelsSpeeds;                   //speeds for each level
        int levelIndx;                              //current level index
    
        int matrixSize;                             //size of the current matrix (a matrix is matrixSize * matrixSize)
        int gameSize;                               //size of the current matrix in the 3D world, (considering also the cellSize and the spaces)
    
        bool pause;
        bool musicOn;
    
        int delay;
        int time;                                   //allows more control respect to ofGetElapsedTimef(). set it to 1 at every level beginning
    
        bool isRotationEnabled;
        int angle;
        int prevAngle;
    
        void nextLevel();
        void repeatLevel();
        vector<vector<vector<Cell>>> levelsParser(ofBuffer buffer);
        string levelChecker(vector<vector<Cell>> level);
    
    public:
        void setup();
        void update();
        void draw();
        void drawGUI();
        void keyPressed(ofKeyEventArgs& eventArgs);
        void loadLevels();
        int getGameSize();
        void exit(ofEventArgs&);
        void audioOut(float * output, int bufferSize, int nChannels);
    
};
