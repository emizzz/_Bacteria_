#pragma once
#include "ofMain.h"
#include "Cell.hpp"
#include "Player.hpp"
#include "Rocket.hpp"


/*--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**

 ENVIRONMENT
 The Environment Class renders the level's grid, the enemies, the player and the rocket. It updates also the cellular automata (births and deaths of the enemies cells).
 
 The methods are:
 
 -setup() => it initializes the environment, the player and the rocket
 -update() => it updates the grid, the rocket and the player and checks for collisions
 -draw() => it draws the grid, the player and the rocket
 -gameOfLifeEngine() => Conway's Game of Life rules
 -countNeighbours() => it counts a cell's neighbors
 -control() => it handles the rocket's and player's commands
 -wallsCollision() => it checks for walls collisions
 -playerCollision() => it checks for player collisions
 -countAliveCells() => it counts the matrix's alive cells
 -getCellSize() => it returns the cell's size
 -getBoolLifeMatrix() => it doesn't return the Cell's matrix, but a boolean's matrix (alive/dead cells)
 
 --**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--***/

class Environment{
    private:
        const int cellSize = 1;                                     //size of a cell
        int gridSize;                                               //size n of the n * n matrix
        vector<vector<Cell>> lifeMatrix;                            //the game's grid
        Player player = Player(ofPoint(0, 0, cellSize), cellSize);
        Rocket rocket = Rocket(ofPoint(0, 0, cellSize), cellSize);
    
        bool wallsCollision(ofPoint cell);
        bool playerCollision(ofPoint cell);
        void gameOfLifeEngine();
        int countNeighbours(vector<vector<Cell>> &matrix, ofPoint _pos, string _mode="xy");
    
    public:
        void setup(vector<vector<Cell>> level);
        void update(bool updateMatrix);
        void draw();
        void control(string control);
        int countAliveCells();
        int getCellSize();
        bool isPlayerAlive();
        vector<vector<bool>> getBoolLifeMatrix();
};
