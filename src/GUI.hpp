#pragma once
#include "ofMain.h"
#include "ofxGui.h"

/*--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
 GUI

 This class uses ofxGui addon ( https://github.com/cwright/ofxGui ).
 
 GUI handles the game's GUI (it appears on the screen when the game is paused).
 With the GUI the user can: starts the game, sets the music on/off and checks the game's rules.
 
 The methods are:
 
 -setup() => it setups the gui, the buttons and the gui's strings
 -update() => it updates positions on screen resizing
 -draw() => it draws the main page or the rules page
 -togglePressed() => event listener (toggle button)
 -buttonPressed() => event listener (buttons)
 -drawTexts() => it draws and positions texts
 -buttonsPosition() => it positions the buttons
 -setMessage() => it sets a message passed by another class
 -setLevel() => it sets the level message
 
 --**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--***/


class GUI{
    
    private:
        ofxPanel gui;                           //the main page
        ofxPanel rulesPage;                     //the rules page
    
        ofxButton playButton;
        ofxButton rulesButton;
        ofxButton backButton;
        ofxToggle musicButton;                  //ofxToggle is a toggle button
    
        bool *pause;
        bool *music;
    
        string title;
        string message;
        string level;
        string rulesText;
        ofTrueTypeFont font;
        const int margin = 20;
        const int buttonHeight = 40;
        const int buttonWidth = 150;
        int initScreenWidth;
        int screenWidth;
        int screenHeight;
        bool rulesPageOpen = false;
    
    
        void togglePressed(const void * sender, bool & pressed);
        void buttonPressed(const void * sender);
        void drawTexts();
        void buttonsPosition();
    
    public:
        void setup(bool &_pause, bool &_music);
        void update();
        void draw();
        void setMessage(string _message);
        void setLevel(string _level);
};
