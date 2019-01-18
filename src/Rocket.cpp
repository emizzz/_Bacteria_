#pragma once
#include "Rocket.hpp"


Rocket::Rocket(ofPoint pos, int size): Cell(pos, size){
    shooting = false;
    colors = {ofColor(20, 20, 20), ofColor(255, 0, 0)};
    
}

/*
UPDATE
 Overrided method!
 
 If the rocket is fired, it moves in the direction "dir" until it collides with a wall or it reaches an enemy.
 If the rocket is not fired, it moves according to the _pos (or parentPos) position.
 
*/
void Rocket::update(ofPoint _pos, ofPoint _dir){
    
    if(shooting){
        pos.x += (dir.x * size * 2);
        pos.y += (dir.y * size * 2);
    }
    else{
        dir = _dir;
        parentPos = _pos;
        pos = parentPos;
    }
    body.setPosition(pos);
    
}

//if the "space" control is sended, this method gives birth to this cell, but only if the event is not already triggered
void Rocket::controls(string control){
    if(control == "space" && !shooting){
        giveBirth();
    }
}

//same parent's method, with the addition of the shooting = true line
void Rocket::giveBirth(){
    alive = true;
    for(int i=0; i<6; i++){
        body.setSideColor(i, colors[1]);
    }
    shooting = true;
}

//same parent's method, with the addition of the shooting = false line
void Rocket::kill(){
    alive = false;
    for(int i=0; i<6; i++){
        body.setSideColor(i, colors[0]);
    }
    shooting = false;
    
    /*
    The following line avoids a bug that happens very rarely. If in the update method the "kill" method is called, in the time between this call and the next "update" call (that decides if the rocket continues or not) can be triggered the "controls" method, that allows the rocket to "reborn", to set the shooting var to true and continue to its path. Consequently the "tempLifeMatrix[prevMapRocketPos.x][prevMapRocketPos.y].giveBirth()" line in the Environment class produces an EXC_BAD_ACCESS.
    */
    pos = parentPos;
}

ofPoint Rocket::getDirection(){
    return dir;
}

