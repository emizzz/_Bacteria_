#include "Soundtrack.hpp"

//it sets the initial soundMatrix
void Soundtrack::setup(vector<vector<bool>> matrix){
    setMatrix(matrix);
}

//it sets the soundMatrix. If the level changes size, also the keyboard should change the number of keys.
void Soundtrack::setMatrix(vector<vector<bool>> matrix){
    soundMatrix.clear();
    soundMatrix = matrix;
    
    if(verticalKeyboard.size() != soundMatrix[0].size()) setVerticalKeyboard(soundMatrix.size());
    
}

/*
 SETVERTICALKEYBOARD
 
 This method is called when the passed matrix changes the previous size.
 The Keys frequencies are selected here. It's important to notice that the harmonic series is an arithmetic series (1×f, 2×f, 3×f, 4×f, 5×f, ...)
 
 */
void Soundtrack::setVerticalKeyboard(int size){
    verticalKeyboard.clear();
    
    for(int y=0; y<size; y++){
        int freq = initFreq * (y % numOfFreq + 1);      //y goes from 0 to numOfFreq-1 cyclically
        
        Key key = Key(freq);
        verticalKeyboard.push_back(key);
    }
    
}

//it sets the keyboard of the current selected (time) column. If the enemy is alive, the key is pressed.
void Soundtrack::setToPlayKeys(int indx){
    for(int y=0; y<soundMatrix[indx].size(); y++){
        if(soundMatrix[indx][y] == true) verticalKeyboard[y].on();
        else verticalKeyboard[y].off();
    }
}

/*
PLAY
 
 The play method is where the sound output is sended to the openFrameworks sound handlers.
 It is called in the ofApp's audioOut method!
 
 The 3 passed parameters are the same as the audioOut() method.
 
 The soundtrack's timer is different from the game's timer, so the soundtrack's time is not constrained to the game's time.
 
 The for loop considers the buffer structure [left ch, right ch, left ch, right ch, ...]
 
*/

void Soundtrack::play(float *output, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize * nChannels; i += 2) {
        double currentFrame = 0;
        
        /*
         metronome that ticks n times a second
         "beatsPerSecond" times per second the currentCount becomes 1
        */
        currentCount=(int)timer.phasor(beatsPerSecond);
        
        if(currentCount != 0){                              //if it is 1
            
            setToPlayKeys(currentToPlayColumnIndx);         //on-off sounds wrt to the game matrix
            
            //the currentToPlayColumnIndx goes from 0 to soundMatrix.size()-1
            if(currentToPlayColumnIndx<soundMatrix.size()-1) currentToPlayColumnIndx++;
            else currentToPlayColumnIndx = 0;
        }
        
        //sum the frequencies (and /verticalKeyboard.size() decreases the total volume)
        for(int y=0; y<verticalKeyboard.size(); y++){
            currentFrame += verticalKeyboard[y].play()/verticalKeyboard.size();
        }
        
        //mix the sound in a stereo mode
        mix.stereo(currentFrame, outputs, 0.5);                     //the parameters pointers are passed
        output[i]=outputs[0];                                       //channel 1
        output[i+1]=outputs[1];                                     //channel 2
    }
}

//it closes the sound stream
void Soundtrack::SoundtrackClose(){
    ofSoundStreamClose();
}
