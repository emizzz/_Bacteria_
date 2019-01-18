#pragma once
#include "ofMain.h"
#include "Cell.hpp"

/*--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
 
 ROCKET
 The Rocket class is a child of the Cell class. It is a cell, but it is triggered by the user.
 The rocket is fired by the user, but it continues in the player's direction until it finds a wall or an enemy. In the event of a collision, it becomes an enemy cell.
 
 The methods are:
 
 -Rocket() => it calls the parent contructor and overrides the colors
 -update() => it updates the rocket's position
 -controls() => it allows the user to shots the rocket
 -giveBirth() => same parent's method, with the addition of the shooting = true
 -kill() => same parent's method, with the addition of the shooting = false
 -getDirection() => it returns the rocket's direction
 
 --**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--***/


class Rocket : public Cell{
    private:
        bool shooting;                                  //true if the user fires this event
        ofPoint dir;                                    //direction
        ofPoint parentPos;                              //the position of the linked cell (in this case the player cell)

    public:
        Rocket(ofPoint pos, int size);
        void update(ofPoint pos, ofPoint dir);          //overrided method
        void controls(string control);
        void giveBirth();                               //overrided method
        void kill();                                    //overrided method
        ofPoint getDirection();

    
};
 
