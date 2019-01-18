#pragma once
#include "ofMain.h"
#include "Cell.hpp"

/*--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
 
 PLAYER
 The Player class is a child of the Cell class. It is a cell, but it is controlled by the user.
 
 The methods are:
 
 -Player() => it calls the parent contructor, overrides the colors and gives birth to the cell
 -controls() => it allows to change the Player's direction
 -getDirection() => it returns the Player's direction
 
 --**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--***/


class Player : public Cell{
    private:
        ofPoint dir = ofPoint(0, 1);                    //direction
    
    public:
        Player(ofPoint pos, int size);
        void controls(string control);
        ofPoint getDirection();
    
};
