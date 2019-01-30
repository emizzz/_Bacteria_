#include "ofApp.h"

void ofApp::setup(){
    bgColor = ofColor(0, 0, 0);

    // adding some OF settings
    ofBackground(bgColor);
    ofSetFrameRate(frameRate);
    ofSetVerticalSync(true);    //Avoid tearing
    //ofEnableLighting();

    game.setup();
    
    // the cam is positioned z in "vertical" and y "backwards" (farther in space => z negative values)
    int gameSize = game.getGameSize();
    cam.setPosition(ofPoint(0, -gameSize, gameSize));
    cam.lookAt(ofPoint(0, 0, 0));                     // (0,0,0) is the center because the Game class use an ofTranslate
    
    // the spotlight has the same pos of the cam and looks at the center of the game.
    light.enable();
    light.setSpotlight();
    light.setPosition(cam.getPosition());
    light.lookAt(ofPoint(0, 0, 0));
    
    
    //enable the audio stream. Params are: out channels, in channels, s.r., b.s., and number of buffers to queue.
    ofSoundStreamSetup(2, 0, this, sampleRate, bufferSize, 4);
    
}

void ofApp::update(){
    
    //the cam and light position is constantly setted because levels could have different sizes
    int gameSize = game.getGameSize();
    cam.setPosition(ofPoint(0, -gameSize, gameSize));
    light.setPosition(cam.getPosition());
    
    game.update();
    
}

void ofApp::draw(){
    /*
    the GUI should be drawn outside the cam POV, the setted lights and the 3D perspective (the last because the gui's labels don't work with the depthTest enabled)
    */
    game.drawGUI();
    
    ofEnableDepthTest();        //depth perception (behind objects are hidden)
    ofEnableLighting();

    cam.begin();
    game.draw();
    cam.end();

    ofDisableLighting();
    ofDisableDepthTest();
    
    
}

void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {
    game.audioOut(output, bufferSize, nChannels);
    
}
