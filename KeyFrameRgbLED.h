// KeyFrameRgbLED.h

#ifndef KeyFrameRgbLED_h
#define KeyFrameRgbLED_h

#include "Arduino.h"
#include "KeyFrame.h"
#include "RgbLED.h"

class KeyFrameRgbLED
{
public:
     KeyFrameRgbLED(int ledRedPin, int ledGreenPin, int ledBluePin, KeyFrameRgb keyFrame[], int numFrames);

    void start();
    void loop();
    bool isAnimationFinished();

protected:

private:
    // current time from start time to now
    unsigned long getRuntime();

    // update current and previous key frames
    void updateCurrentKeyFrame();

    // calculates the current Speed
    void calculateCurrentSpeed();

    RgbLED _rgbLed;
    KeyFrameRgb* _keyFrame;
    int _numFrames;
    int _currentFrameIdx;
    double _currentRedSpeed;
    double _currentGreenSpeed;
    double _currentBlueSpeed;

    unsigned long _startTime;

    bool _animationActive;

    KeyFrameRgb _previousKeyFrame;
    KeyFrameRgb _currentKeyFrame;
     
};

#endif 