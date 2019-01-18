#include "Player.hpp"


//this method calls the parent method, but overrides the colors and gives birth to the cell from the beginning.
Player::Player(ofPoint pos, int size): Cell(pos, size){   //call to the constructor with pos and size parameters
    colors = {ofColor(20, 20, 20), ofColor(255, 197, 0)};
    giveBirth();
}

/*
CONTROLS

 This method allows the user to control the Player's direction. The direction and the position are updated.

*/
void Player::controls(string control){
    ofPoint currentDir = dir;         

    if(control == "left"){                  //90° counterclockwise rotation
        currentDir.x = -dir.y;
        currentDir.y = dir.x;
    }
    else if(control == "top"){              //0° counterclockwise rotation
        currentDir.x = dir.x;
        currentDir.y = dir.y;
    }
    else if(control == "right"){            //270° counterclockwise rotation
        currentDir.x = dir.y;
        currentDir.y = -dir.x;
    }
    
    dir = currentDir;
    pos.x += (dir.x * size * 2);            //the player "jump" over the grid's spaces
    pos.y += (dir.y * size * 2);
    
}

ofPoint Player::getDirection(){
    return dir;
}
