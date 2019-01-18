#include "GUI.hpp"

/*
 SETUP
 
 In this method the GUI settings, the buttons and the strings is setted.
*/
void GUI::setup(bool &_pause, bool &_music){
    
    pause = &_pause;
    music = &_music;
    initScreenWidth = ofGetScreenWidth();
    title = "-*- BACTERIA -*-";
    message = "";
    level = "";
    
    //the 2 bools are AntiAliased and FullCharacterSet
    font.load("UbuntuMono-Regular.ttf", initScreenWidth/80, true, true);
    ofxGuiSetFont("UbuntuMono-Regular.ttf", initScreenWidth/80);
    
    rulesText = "Kill all the enemies. They reproduce with the following rules: \n\n"
    "a) Each alive cell with one or no neighbors dies, as if by solitude. \n"
    "b) Each alive cell with four or more neighbors dies, as if by overpopulation. \n"
    "c) Each alive cell with two or three neighbors survives. \n"
    "d) Each dead cell with three neighbors becomes populated. \n\n"
    "Warning, enemies can pass cross the borders, but you can't! \n\n"
    "Controls are: \n\n"
    "- left-up-right arrow or A W and D for move the player \n"
    "- space bar for shooting \n"
    "- p for pause the game \n";
    

    //Main GUI (3 buttons)
    gui.setup();
    
    gui.setShape(0, 0, initScreenWidth, ofGetScreenHeight());
    gui.setBorderColor(ofColor(0, 0, 0));
    
    gui.add(playButton.setup("Play", buttonWidth, buttonHeight));
    gui.add(rulesButton.setup("Rules", buttonWidth, buttonHeight));
    gui.add(musicButton.setup("Music", music, buttonWidth, buttonHeight));
    
    
    //Second GUI's page (rules)
    rulesPage.setup();
    
    rulesPage.setShape(0, 0, initScreenWidth, ofGetScreenHeight());
    rulesPage.setBorderColor(ofColor(0, 0, 0));
    
    rulesPage.add(backButton.setup("Back", buttonWidth, buttonHeight));
    

    //buttons listeners
    playButton.addListener(this, &GUI::buttonPressed);
    rulesButton.addListener(this, &GUI::buttonPressed);
    backButton.addListener(this, &GUI::buttonPressed);
    musicButton.addListener(this, &GUI::togglePressed);
    
    
}
void GUI::update(){
    gui.setPosition(0,0);           //workaround for deleting the drag option
    rulesPage.setPosition(0,0);     //workaround for deleting the drag option
    
    /*
     if the user resizes the window, the gui adjusts its size according to the new size (I don't use the ofApp::windowResized because I should pass it throught the Game class)
    */
    if(ofGetWindowWidth() != screenWidth || ofGetWindowHeight() != screenHeight){
        
        screenWidth = ofGetWindowWidth();
        screenHeight = ofGetWindowHeight();
        gui.setSize(screenWidth-margin*2, screenHeight-margin*2);           //set size to the main page
        rulesPage.setSize(screenWidth-margin*2, screenHeight-margin*2);     //set the size to the rules page
        
        buttonsPosition();
        
    }
}
void GUI::draw(){
    
    if(rulesPageOpen) rulesPage.draw();
    else gui.draw();

    drawTexts();

}

//buttons listeners
void GUI::buttonPressed(const void * sender){
    ofParameter<void> * button = (ofParameter<void> *)sender;
    string name = button->getName();        //it allows to use a single method for more buttons
    
    if(name == "Play"){
        *pause = false;
    }
    else if(name == "Rules"){
        rulesPageOpen = true;
    }
    else if(name == "Back"){
        rulesPageOpen = false;
    }
    
}

//toggle button is a special type of button
void GUI::togglePressed(const void * sender, bool & pressed){
    *music = pressed;
}

//it draws the 2 pages texts
void GUI::drawTexts(){
    
    if(rulesPageOpen){
        ofPushMatrix();
        ofTranslate(margin*3, margin*5);
        ofScale((float)screenWidth/initScreenWidth, (float)screenWidth/initScreenWidth, 1);
        font.drawString(rulesText, 0, 0);
        ofPopMatrix();
    }
    else{
        font.drawString(title, screenWidth/2 - font.stringWidth(title)/2, margin*3);
        font.drawString(level, screenWidth/2 - font.stringWidth(level)/2, margin*6);
        font.drawString(message, screenWidth/2 - font.stringWidth(message)/2, margin*8);
    }

}

//it sets the position of the main page's button
void GUI::buttonsPosition(){
    
    ofPoint topLeftCorner(screenWidth/2-buttonWidth/2, margin*10);

    playButton.setPosition(topLeftCorner);
    musicButton.setPosition(ofPoint(topLeftCorner.x, topLeftCorner.y + 50));
    rulesButton.setPosition(ofPoint(topLeftCorner.x, topLeftCorner.y + 100));

}

//it sets a message from the game's class (for example if the user wins)
void GUI::setMessage(string _message){
    message = _message;
}

//it sets the game's level
void GUI::setLevel(string _level){
    level = "Level " + _level;
}

