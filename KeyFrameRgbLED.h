// KeyFrameRgbLED.h

#ifndef KeyFrameRgbLED_h
#define KeyFrameRgbLED_h

#include "Arduino.h"

#include "Helpers.h"
#include "KeyFrame.h"
#include "RgbLED.h"
#include "RGB.h"

class KeyFrameRgbLED
{
public:
     KeyFrameRgbLED(int rgbId, KeyFrameRgb keyFrame[], int numFrames);

    void start();
    void loop();
    bool isAnimationFinished();
    bool needsUpdate();
    RGB getCurrentColor();
    int getId();

protected:

private:
    // current time from start time to now
    unsigned long getRuntime();

    // update current and previous key frames
    void updateCurrentKeyFrame();

    // calculates the current Speed
    void calculateCurrentSpeed();

    RgbLED _rgbLed;
    int _ledId;
    KeyFrameRgb* _keyFrame;
    int _numFrames;
    int _currentFrameIdx;
    
    RGB    _currentColor;
    double _currentRedSpeed;
    double _currentGreenSpeed;
    double _currentBlueSpeed;

    unsigned long _startTime;

    bool _animationActive;
    bool _needsUpdate;

    KeyFrameRgb _previousKeyFrame;
    KeyFrameRgb _currentKeyFrame;
     
};

#endif 
