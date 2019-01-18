#pragma once
#include "ofMain.h"

/*--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
 
 CELL
 The Cell class represents a cell in the game's grid. It is the parent class of Player and Rocket.
 Enemies, the grid, the player and the rocket are cells.
 
 The methods are:
 
 -Cell() => it creates the dead cell
 -update() => it updates the cell's position
 -draw() => it draws the cell
 -giveBirth() => it gives birth to the cell
 -kill() => it kills the cell
 -isAlive() => it returns the cell's state
 -setPos() => it sets the cell's position
 -getPos() => it returns the cell's position
 
 --**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--***/


class Cell{
    
    protected:
        ofPoint pos;                        //position
        bool alive;                         // 1 == alive   0 == dead
        int size;
        ofBoxPrimitive body;
        vector<ofColor> colors;             // 2 cell's colors: the first is used when the cell is dead, the second when it is alive
    
    public:
        Cell(ofPoint _pos, int size = 1);
        void update();
        void draw();
        void giveBirth();
        void kill();
        bool isAlive();
        void setPos(ofPoint _pos);
        ofPoint getPos();
    
};
