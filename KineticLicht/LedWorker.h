//LedWorker.h

#ifndef LedWorker_h
#define LedWorker_h

#ifdef WITHIN_UNITTEST
  #include <stdlib.h>
  #include <stdio.h>
#else
  #include <Arduino.h>
  #include <ArduinoSTL.h>
#endif

#include "KeyFrame.h"
#include "RGB.h"

class LedWorker
{
public:
    LedWorker(int rgbId);

    void init(); // to be called at setup()
    void loop(long elapsedTime); // to be called in loop();
    // update current key frames
    void updateTargetKeyFrame(long elapsedTime, KeyFrame& kf);
    bool needsUpdate();
    RGB  getColorForUpdate();
    int  getId();
    bool hasPassedTargetKeyFrame();

    void setDebug(bool debug);

private:
    // calculates the current Speed
    void calculateCurrentSpeed();
    void checkAnimation(long elapsedTime);

    // currently the id used for the TLC5947
    int _ledId;

    // actual color on the LED
    RGB    _currentColor;
    // expected color on the LED
    RGB    _expectedColor;

    double _currentRedSpeed;
    double _currentGreenSpeed;
    double _currentBlueSpeed;

    int _needsUpdateDelta;
    bool _pastTargetKeyFrame;
    bool _needsUpdate;

    bool _debug;

    KeyFrame _previousKeyFrame;
    KeyFrame _targetKeyFrame;

};

#endif
