#include "Game.hpp"

void Game::setup(){
    logFileName = "log.txt";
    pause = true;
    musicOn = true;
    //delay = 240;
    time = 1;                                   //1, so the level doesn't update immediately
    isRotationEnabled = true;
    angle = 0;
    prevAngle = angle;
    
    ofLogToFile(logFileName, true);
    levelIndx = -1;                             //the init level is -1, but becomes 0 when I call nextLevel()
    loadLevels();
    
    //pause and musicOn vars are passed by reference. These values are directly changeable from the GUI.
    gui.setup(pause, musicOn);
    
    nextLevel();                                //it calls environment.setup()
    
    /*add an event listener
     (the ofEvent that we want to listen to, pointer to the class that is going to be listening, pointer to the method that's going to be called)*/
    ofAddListener(ofEvents().keyPressed, this, &Game::keyPressed);
    ofAddListener(ofEvents().exit, this, &Game::exit);
    
}

/*
UPDATE
 
 This method handles the game's logic. This is the flow:
 
 if the game is stopped (paused) => display the GUI
 if the game is not stopped =>
 
    if the player is alive =>
        if DELAYED UPDATE (update the game every tot sec) =>
            stop the game if the player won
            otherwise update the game (and update the enemies's positions)
        if EVERY UPDATE =>
            update the game (without update the enemies's positions)
 
     if the player is not alive =>
        stop the game
 
*/
void Game::update() {
    
    if (!pause) {
        if (environment.isPlayerAlive()) {
            
            /*if delay is 240, "time % delay" goes cyclically from 0 to 239, it allows to update this code part only
             every 240 cycles.*/
            if (time % delay == 0) {
                time = 0;                                               //with this, time does not grow too much...
                int aliveCells = environment.countAliveCells();
                
                //if the level is finished and is not the last level
                if (aliveCells == 0 && levelIndx < levels.size() - 1) {
                    gui.setMessage("You win!");
                    nextLevel();
                    pause = true;
                }
                //if the level is finished and is the last level
                else if (aliveCells == 0 && levelIndx == levels.size() - 1) {
                    pause = true;
                    gui.setMessage("Game finished!");
                }
                //if the player is playing
                else {
                    environment.update(true);          //the enemies's position (or rather the game's matrix) is updated with the TRUE parameter
                    gui.setMessage("");
                    
                    /*if the musicOn var is true, the game matrix is passed to the Soundtrack class, that treats it like a kind of Keyboard (or rather a sequencer)*/
                    if (musicOn) soundtrack.setMatrix(environment.getBoolLifeMatrix());
                }
            }
            else {
                //the environment is updated, but not the enemies's positions.
                environment.update(false);
                
                /*
                 if the angle is 0, 90, 180, 270 or 360 there isn't rotations, else the
                 environment is rotating.
                 */
                if (isRotationEnabled) {
                    if (angle % 90 != 0) {
                        if (angle > prevAngle) angle += 5;      //counterclockwise rotation
                        else angle -= 5;                        //clockwise rotation
                    } else {
                        if (abs(angle) == 360) angle = 0;       //the angle is limited from -360° to 360°
                    }
                }
            }
            time++;
            
        // if the player is dead
        } else {
            gui.setMessage("You are dead!");
            pause = true;
            repeatLevel();
        }
        
    //if the pause var is TRUE
    } else {
        gui.update();
    }
    
}


void Game::draw(){
    
    if(!pause){
        ofHideCursor();                                 //hide the cursor during the game
        ofPushMatrix();
        
        ofRotateZDeg(angle);                            //rotate angle° around the Z axis (the axis "vertical" to the grid)
        ofTranslate(-gameSize/2, -gameSize/2);          //center the environment (the rotation happens in the (0,0,0) )
        
        environment.draw();
        
        ofPopMatrix();
    }
    else{
        ofShowCursor();                                 //show the cursor in the GUI
    }
    
}
/*
 DRAWGUI

 this method should be called out of the cam scope (because the GUI doesn't need 3D and lights)
 
 */
void Game::drawGUI(){
    if(pause){
        gui.draw();
    }
}

/*
KEYPRESSED
 
 This method allows to send to the environment the 4 game's commands: "up", "left", "right" and "space".
 Events (apart from pause) are triggered only if a rotation is completed.
 The keys are:
    -UP => arrow up or W
    -LEFT => left arrow or A
    -RIGHT => right arrow or D
    -SPACE => spacebar
    -PAUSE => p
 
 */
 void Game::keyPressed(ofKeyEventArgs& eventArgs){
    
    //debug
    int key = eventArgs.key;
    
    if(!pause){
        if(angle % 90 == 0){                             //it doesn't allow to rotate several times in the same action
            
            prevAngle = angle;                           //store the prevAngle var immediately after the key press (prevAngle allows to understand the rotation direction)
            
            if(key == 57357 || key == 119) environment.control("up");
            if(key == 57356 || key == 97){
                environment.control("left");
                
                if(isRotationEnabled){
                    /*the rotation is triggered by the new angle (that isn't divisible by 90)
                     clockwise rotation (-)
                    */
                    angle-=5;
                }
            }
            if(key == 57358 || key == 100){
                environment.control("right");
    
                if(isRotationEnabled){
                    angle+=5;                               //counterclockwise rotation (+)
                }
            }
            if(key == 32) environment.control("space");
        }
    }
    
    if(key == 112){         // "p" (pause) key
        pause = !pause;
    }
    
}

/*
 LOADLEVELS
    This method allows to load the game levels. The levels are simple 0 and 1 matrixes stored in a txt file in the bin/data folder.
 
    This method:
    1) loads the txt data
    2) parses the data with the levelParser() method
    3) checks for some common errors in these matrices with the levelChecker() method
    4) sets only the valid levels, return a 1*1 grid if all the levels are not valid (or the file is not present)
 
     Checks:
        -does the file exist? (loadLevels method)
        -are the level's header correct? (levelsParser method)
        -are the matrix's numbers valid? (0 or 1) (levelsParser method)
        -are the levels square shaped? (levelChecker)
        -are the levels non empty (with at least one "1")? (levelChecker method)
 
 */
void Game::loadLevels(){
    vector<vector<vector<Cell>>> loadedLevels;
    vector<vector<vector<Cell>>> validLevels;               //if there is errors, this will contains only the valid levels
    vector<int> validSpeeds;                                //speeds share the same level's index, so if it changes, it must be changed also the speed indexes

    bool isValid = true;                                    //if any level is not valid this will be false
    
    ofBuffer buffer = ofBufferFromFile("levels.txt");       //fetches the data from the levels.txt file
    
    loadedLevels = levelsParser(buffer);                    //parses the txt file, returns a matrix for each level
    
    if(loadedLevels.size() == 0){                           //if the file isn't correctly parsed, create a 1*1 game's grid
        isValid = false;
        levelsSpeeds.clear();
        levelsSpeeds.push_back(240);                        //default delay
        int cellSize = environment.getCellSize();
        Cell newCell = Cell(ofPoint(0, 0, cellSize), cellSize);
        validLevels.push_back(vector<vector<Cell>>());
        validLevels[0].push_back(vector<Cell>());
        validLevels[0][0].push_back(newCell);
        
        ofLogError() << "File levels.txt missing. Check in the data folder." << endl;
    }
    else{                                                  //if the file is correctly parsed
        string tempError = "";
        for(int l=0; l<loadedLevels.size(); l++){
            tempError = levelChecker(loadedLevels[l]);       //checks if the data is well structured for each level
            
            if(tempError != ""){
                isValid = false;
                ofLogError() << tempError << " (level index:  " << l << ")" << endl;
            }
            else{
                validLevels.push_back(loadedLevels[l]);         //contains only the valid levels
                validSpeeds.push_back(levelsSpeeds[l]);
            }
        }
    }
    
    if(!isValid){
        levelsSpeeds.clear();
        levelsSpeeds = validSpeeds;
    }
    levels = validLevels;
    
}

/*
LEVELPARSER
 This method takes the txt-file's buffer, and returns a vector of matrixes (or a vector of game's grids).
 
 The delay can't be < 60, if I set it < 60, it remains 60.
 If the delay is not a valid integer, the delay is set to the default (240).
 In both cases the game continues without interruptions.
 
 The matrix has the structure: [col[row, row,...], col[row, row,...], ...], so it can be called like this: "matrix[x][y]" rather than this: "matrix[y][x]"
 
 */
vector<vector<vector<Cell>>> Game::levelsParser(ofBuffer buffer){
    vector<vector<vector<Cell>>> loadedLevels;
    int cellSize = environment.getCellSize();
    int y = 0;
    
    for (auto line : buffer.getLines()){
        
        //parse spaces and the level's header
        if(line[0] == '\0') continue;                                   // \0 => marks the end of the string
        if(line[0] == '#' and line[1] == '#'){                          // line stars with ## (level's header)
            unsigned int delay = 240;                                   //default delay
            y = 0;
            loadedLevels.push_back(vector<vector<Cell>>());             //add a void level's grid
            
            try{                                                        //try to convert the "delay" to int
                vector<char> delayVec(line.begin() + 8, line.end());
                string delayStr( delayVec.begin(), delayVec.end() );
                delay = max(stoi(delayStr), 60);                        //we can't set the levels delay < 60
            }
            catch(...){                                                 // "..." catches everything.
                ofLogError() << "Wrong sintax in a level declaration. This is an example of how should be the line before the level: ##delay=240" << endl;
            }
            levelsSpeeds.push_back(delay);                              //append the delay to the levelsSpeeds vector ( the loadedLevels[n] has a delay of levelsSpeeds[n] )
            
        }
        //parse the level's matrix
        if(line[0] == '0' || line[0] == '1'){
            vector<string> row = ofSplitString(line, ",");                      //take the current line (row)
            
            for(int x=0; x<row.size(); x++){
                Cell newCell = Cell(ofPoint(x * cellSize*2, y * cellSize*2, cellSize), cellSize);
                
                try{
                    if(stoi(row[x]) == 1) newCell.giveBirth();
                }
                catch(...){
                    ofLogError() << "No correct value in the level matrix: " <<  loadedLevels.size()-1 << endl;
                }
                
                /*
                when the parser processes the first row, it adds a row for every value in this row.
                Then (when the y value is no longer 0) it assigns the new cell value to the respective row.
                It allows to have this structure: [col[row, row,...]
                */
                if(y == 0){
                    loadedLevels[loadedLevels.size()-1].push_back(vector<Cell>());  //for each col insert a row
                }
                loadedLevels[loadedLevels.size()-1][x].push_back(newCell);
            }
            y++;
            
        }
    }
    return loadedLevels;
    
}

/*
 LEVELCHECKER
 This method takes a level's grid in input, and returns a message if it contains one of the handled errors,  otherwise it returns an empty string.
 
 */
string Game::levelChecker(vector<vector<Cell>> level){
    string message = "";
    int aliveCells = 0;
    
    for(int x=0; x<level.size(); x++){  //n. columns
        
        //it checks if the level's matrix shape is a square
        if( level[x].size() != level.size() ){                      // n. rows != n. columns ?
            message =  "error in levels.txt. Levels must have a square shape (NxN).";
        }
        
        //checks if there aren't alive cells within the level
        for(int y=0; y<level[0].size(); y++){                       //n. rows
            if(level[x][y].isAlive()) aliveCells++;
        }
    }
    
    if(aliveCells == 0){                                 //if there aren't alive cells, the level is not valid
         message =  "The levels must have at least 1 alive cell.";
    }
    
    return message;
    
}

/*
NEXTLEVEL
 
 This method is called when a level is finished. It:
    -increments the level index and selects the new grid's level
    -gets the new size (used to move the camera) and the new delay
    -resets the angle (POV) and the time
    -setups the GUI and the new environment
    -resets the soundtrack's matrix

*/
void Game::nextLevel(){
    levelIndx++;
    matrixSize = levels[levelIndx].size();
    gameSize = (matrixSize + matrixSize-1) * environment.getCellSize();     //(boxes + spaces) * box's size
    delay = levelsSpeeds[levelIndx];
    
    angle = 0;                                                              //reset the POV
    time = 1;                                                               //reset the timer (so the player starts before a fixed update's time)
    
    environment.setup(levels[levelIndx]);                                   //SETUP THE NEW ENVIRONMENT
    gui.setLevel(to_string(levelIndx));
    if(musicOn) soundtrack.setMatrix(environment.getBoolLifeMatrix());      //reset the "music"
    
}

/*
 REPEATLEVEL
 This method is similar to nextLevel() but the levelIndex is not incremented, and some variables remains the same.
*/
void Game::repeatLevel(){
    
    angle = 0;                                                  //reset the POV
    time = 1;                                                   //reset the timer
    
    environment.setup(levels[levelIndx]);
    if(musicOn) soundtrack.setMatrix(environment.getBoolLifeMatrix());  //reset the "music"
}

int Game::getGameSize(){
    return gameSize;
}

void Game::exit(ofEventArgs&){
    soundtrack.SoundtrackClose();                               //this avoids some errors closing the app
}

void Game::audioOut(float * output, int bufferSize, int nChannels){
    if(!pause && musicOn){
        soundtrack.play(output, bufferSize, nChannels);         //pass the audioOut parameters to the Soundtrack class
    }
}
