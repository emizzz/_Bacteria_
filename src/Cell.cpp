#include "Cell.hpp"

/*
 CELL contructor
 
 When a cell is created, it has a position (mandatory) and a size (not mandatory, default = 1).
 A cell, when is created, is dead.
 
 */
Cell::Cell(ofPoint _pos, int _size){
    pos = _pos;
    size = _size;
    colors = {ofColor(20, 20, 20), ofColor(159, 0, 55)};
    body.set(size);
    body.setPosition(_pos);
    kill();
}

//it updates the cell's position
void Cell::update(){
    body.setPosition(pos);
}


void Cell::draw(){
    body.draw();
}

//it sets alive to TRUE and colors all the body's sides
void Cell::giveBirth(){
    alive = true;
    for(int i=0; i<6; i++){
         body.setSideColor(i, colors[1]);
    }
}

//it sets alive to FALSE and colors all the body's sides
void Cell::kill(){
    alive = false;
    for(int i=0; i<6; i++){
        body.setSideColor(i, colors[0]);
    }
}

void Cell::setPos(ofPoint _pos){
    pos = _pos;
}
ofPoint Cell::getPos(){
    return pos;
}
bool Cell::isAlive(){
    return alive;
}
