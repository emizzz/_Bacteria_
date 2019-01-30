#include "Environment.hpp"

void Environment::setup(vector<vector<Cell>> level){
    
    //set the current level's matrix passed by the Game istance
    lifeMatrix = level;
    gridSize = lifeMatrix.size();
    
    /*
     Creation of the player at position (gridGame/2, 1, 1) of the grid
     The player is alive after this call.
    */
    player = Player(ofPoint(floor(gridSize/2) * cellSize*2, cellSize*2, cellSize), cellSize);
    
    /*
    Creation of the rocket (in the same player position)
    The rocket is not alive after this call, is still hidden.
    */
    rocket = Rocket(player.getPos(), cellSize);
    
}
/*
UPDATE
 
 This is the main game's algorithm. It updates the lifeMatrix (if the updateMatrix param is TRUE), then it checks for collisions with enemies and with the walls (this is called before the player.update() because thanks to this we don't see the player going beyond the limits for a split second)

 Then, the player and the rocket is updated. After this, this method checks for rocket's collisions. This happens after the rocket update because this algorithm use:
        -the current rocket position to check for collisions
        -the last rocket position (before a collision) to transform the rocket into an enemy cell.
 
 The flow is:
    1) updates lifeMatrix (if updateMatrix == true)
    2) checks for "player - walls" collisions
    3) checks for "player - enemies" collisions

    4) updates player and rocket
 
    5) checks for "rocket - walls" collisions
    6) checks for "rocket - enemies" collisions
 
*/
void Environment::update(bool updateMatrix){
    ofPoint prevRocketPos = rocket.getPos();                          //the rocket pos in the physical world
    ofPoint prevMapRocketPos = prevRocketPos/(cellSize*2);            //the rocket pos in the life matrix
    prevMapRocketPos.x = int(prevMapRocketPos.x);
    prevMapRocketPos.y = int(prevMapRocketPos.y);
    prevMapRocketPos.z = int(prevMapRocketPos.z);
    
    
    //GAME OF LIFE engine
    if(updateMatrix){
        gameOfLifeEngine();
    }
    
    /*
     if the player collides with a wall, it goes back in its last position
     (I use the rocket pos. because is the same as the player's pos.)
     */
    if(wallsCollision(player.getPos())){
        player.setPos(prevRocketPos);
    }
    
    //if the player collides with another cell, the player dies.
    if(playerCollision(player.getPos())){ 
        player.kill();
    }
    
    
    //Here is where the player and the rocket are updated.
    rocket.update(player.getPos(), player.getDirection());
    player.update();
    
    
    ofPoint newRocketPos = rocket.getPos();
    ofPoint newMapRocketPos = newRocketPos/(cellSize*2);

    /*
     if the rocket collides with a wall, it dies, and borns a new cell in the last rocket's pos.
    */
    if(wallsCollision(newRocketPos) && rocket.isAlive() ){
        lifeMatrix[prevMapRocketPos.x][prevMapRocketPos.y].giveBirth();
        rocket.kill();
    }
    
    /*
     if the rocket is near an alive cell, it dies, and borns a new cell in the last rocket's pos.
     If the rocket is moving on the x axis, it must considers only the cells on the x axis, the same for the y. This avoids that the cell is appended in an enemy's "neighbourhood angle".
     
     -rocket.getDirection()[0] == 1 => the rocket move in the x direction
     -rocket.getDirection()[0] == 0 => the rocket move in the y direction
    */    
    string mode = abs(rocket.getDirection()[0]) == 1 ? "x" : "y";
    if(countNeighbours(lifeMatrix, newRocketPos, mode) > 0  && rocket.isAlive()){
        lifeMatrix[newMapRocketPos.x][newMapRocketPos.y].giveBirth();
        rocket.kill();
    }

}

//it draws the player, the rocket and the game's grid (with the enemies)
void Environment::draw(){
    
    player.draw();
    rocket.draw();
    
    for(int x=0; x<lifeMatrix.size(); x++){
        for(int y=0; y<lifeMatrix[0].size(); y++){
            lifeMatrix[x][y].draw();
        }
    }
    
}

/*
 GAMEOFLIFEENGINE
 
 This method is the game's core and is based on the Conway's Game of Life rules:
     - Each alive cell with one or no neighbors dies, as if by solitude.
     - Each alive cell with four or more neighbors dies, as if by overpopulation.
     - Each alive cell with two or three neighbors survives.
     - Each dead cell with three neighbors becomes populated.
 
  The method uses a copy of the original lifeMatrix, because if we change directly values in the orginal matrix, the algorithm doesn't work as expected.
*/
void Environment::gameOfLifeEngine(){
    vector<vector<Cell>> matrix = lifeMatrix;
    
    for(int x=0; x<matrix.size(); x++){
        for(int y=0; y<matrix[0].size(); y++){
            int currentNeighbors = countNeighbours(matrix, ofPoint(x*cellSize*2, y*cellSize*2));
            
            if(matrix[x][y].isAlive() && currentNeighbors <= 1){
                lifeMatrix[x][y].kill();
            }
            else if(matrix[x][y].isAlive() && currentNeighbors >= 4){
                lifeMatrix[x][y].kill();
            }
            else if(matrix[x][y].isAlive() && (currentNeighbors == 2 || currentNeighbors == 3)){
                lifeMatrix[x][y].giveBirth();   //it is altready alive...
            }
            else if(!matrix[x][y].isAlive() && currentNeighbors == 3){
                lifeMatrix[x][y].giveBirth();
            }
        }
    }
}

//if the player's position fits with an enemy's position, it returns true, otherwise false
bool Environment::playerCollision(ofPoint cell){
    ofPoint currentPos = cell/(cellSize*2);                   //map the player pos to the matrix index
    
    if(lifeMatrix[currentPos.x][currentPos.y].isAlive()) return true;
    return false;
}

//if the passed position is outside the grid, returns true, otherwise false.
bool Environment::wallsCollision(ofPoint cell){
    if( (cell.y < 0 || cell.y > gridSize*cellSize*2 - cellSize) ||      // *2 because the grid has spaces
        (cell.x < 0 || cell.x > gridSize*cellSize*2 - cellSize) ){
        return true;
    }
    return false;
}

/*
 It counts the neighbors of a given grid position.
 If mode is setted to x or y, only the neighbors in the x or y direction is taken in consideration.
*/
int Environment::countNeighbours(vector<vector<Cell>> &matrix, ofPoint _pos, string _mode){
    
    int count = 0;
    ofPoint currentPos = _pos/(cellSize*2);     //map the pos to matrix's indexes
    int fromX, fromY, toX, toY;
    
    if(_mode == "x"){               //if the mode is "x", the loop goes from -1 to 2 inly in the x direction
        fromX = -1;
        fromY = 0;
        toX = 2;
        toY = 1;
    }
    else if(_mode == "y"){          //if the mode is "y", the loop goes from -1 to 2 inly in the y direction
        fromX = 0;
        fromY = -1;
        toX = 1;
        toY = 2;
    }
    else{                           //otherwise neighbors are checked in both directions
        fromX = -1;
        fromY = -1;
        toX = 2;
        toY = 2;
    }
    
    for(int x=fromX; x<toX; x++){
        for(int y=fromY; y<toY; y++){
            
            ofPoint neighborPos = ofPoint(currentPos.x + x, currentPos.y + y);
            if((neighborPos.x == currentPos.x) && (neighborPos.y == currentPos.y) ) continue; //the current cell is not calculated as a neighbour
            
            /*the famous PACMAN effect*/
            if(neighborPos.x < 0) neighborPos.x = gridSize-1;
            if(neighborPos.y < 0) neighborPos.y = gridSize-1;
            if(neighborPos.x >= gridSize) neighborPos.x = 0;
            if(neighborPos.y >= gridSize) neighborPos.y = 0;
            
            //if this cell is alive (is an enemy), increments the count var
            if(matrix[int(neighborPos.x)][int(neighborPos.y)].isAlive()) count++;
            
        }
    }
    return count;

}

/* utility: it counts the alive cells (we can use also a class attribute, without call every time a method), but I use this method because the code is probably clearer.
*/
int Environment::countAliveCells(){
    int aliveCells = 0;
    for(int x=0; x<lifeMatrix.size(); x++){
        for(int y=0; y<lifeMatrix[0].size(); y++){
            if(lifeMatrix[x][y].isAlive()) aliveCells++;
        }
    }
    return aliveCells;
    
}

//pass events to the player and the rocket
void Environment::control(string control){

    if(control == "up") player.controls("up");
    if(control == "left") player.controls("left");
    if(control == "right") player.controls("right");
    if(control == "space") rocket.controls("space");
    
}

/*TODO: I could pass it as a pointer...*/
//a boolean's matrix is used in the Soundtrack class. Boolean represent the cell's state: alive/dead.
vector<vector<bool>> Environment::getBoolLifeMatrix(){
    vector<vector<bool>> boolMatrix;
    for(int x=0; x<lifeMatrix.size(); x++){
        boolMatrix.push_back(vector<bool>());
        for(int y=0; y<lifeMatrix[0].size(); y++){
            boolMatrix[x].push_back(lifeMatrix[x][y].isAlive() ? true : false);
        }
    }
    return boolMatrix;
}

int Environment::getCellSize(){
    return cellSize;
}

bool Environment::isPlayerAlive(){
    return player.isAlive();
}

