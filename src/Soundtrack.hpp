#pragma once
#include "ofMain.h"
#include "ofxMaxim.h"

/*--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**
 
 SOUNDTRACK
 
 This class uses ofxMaxim addon ( https://github.com/falcon4ever/ofxMaxim ).

 The Soundtrack Class handles the game's soundtrack. The game's grid is treated like a sequencer: the x axis is the time axis and the y axis is like a "keyboard" (or a piano) that plays only harmonics. I choose harmonics because they sound good when played at the same time.
 
 The game's enemies in this class represent the keyboard's keys!
 If they are alive, the keyboard's keys are pressed (and there is a sound), otherwise the keys are not pressed (and there is no sound).
    Every time setMatrix(matrix) is called, the Soundtrack class changes its to play harmonics.
 
        ^
        | 0,0,0,1
        | 0,1,0,0
    FREQ| 0,0,0,0
        | 0,0,1,0
        |----TIME---->
 
 The methods are:
 
 -setup() => it sets the matrix
 -setMatrix() => it sets the matrix
 -play() => it sums all the frequencies and send them to the outputs channels
 -SoundtrackClose() => it closes the soundstream
 -setToPlayKeys() => it sets the on and off keyboard's keys
 -setVerticalKeyboard() => it sets the "keyboard" according to initFreq and numOfFreq
 

 KEY
 Key is a tiny class and it is useful for storing:
    -the Oscillator (repeating waveform with a fundamental frequency)
    -the Envelope (the attack, sustain, and decay of a sound)
 
 
 --**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--***/


class Key{
    private:
        maxiOsc osc;
        maxiEnv env;
        double frequency;
    
    public:
        Key(double _f){
            frequency = _f;
        }
        // Envelope's parameters are: input, attack, release, holdtime, trigger
        inline double play(){
            return env.ar(osc.sinewave(frequency), 0.1, 0.1, 1, env.trigger);
        }
        //after on() is called, if we call play(), we get an oscillating number.
        inline void on(){
            return env.trigger = 1;
        }
        //after off() is called, if we call play(), we get a stable number.
        inline void off(){
            return env.trigger = 0;
        }
};

class Soundtrack{
    
    private:
        const int beatsPerSecond = 8;           //the "sequencer"'s speed
        const int initFreq = 100;               //the lowest frequency
        const int numOfFreq = 5;                //number of used harmonics, if the matrix's height is bigger than this number, the harmonics are repeated( this happens in setVerticalKeyboard() )
    
        maxiOsc timer;                          //the timer oscillator
        int currentCount;
    
        maxiMix mix;                            //stereo bus
    
        int currentToPlayColumnIndx;            //the current "sequencer" column index. If this is 0, the sequencer will play the 0th matrix's column, if this is 5, it will play the 5th, ...
        vector<Key> verticalKeyboard;           //all the keyboard's keys (n. keys == n. matrix's rows)
        vector<vector<bool>>soundMatrix;        //this is the Environment's matrix obtained by calling getBoolLifeMatrix(). True value means keyboard's key pressed, False value means key not pressed.
    
    
        void setToPlayKeys(int indx);
        void setVerticalKeyboard(int size);
    
    public:
        void setup(vector<vector<bool>> matrix);
        void play(float *output, int bufferSize, int nChannels);
        void setMatrix(vector<vector<bool>> matrix);
        void SoundtrackClose();
    
};
