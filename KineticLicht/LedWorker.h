//LedWorker.h

#ifndef LEDWORKER_H
#define LEDWORKER_H

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
    LedWorker(int rgbId, int tlc5947_id);

    void init(); // to be called at setup()
    void loop(long elapsedTime); // to be called in loop();
    // update current key frames
    void updateTargetKeyFrame(long elapsedTime, KeyFrame& kf);
    bool needsUpdate();
    RGB  getColorForUpdate();
    int  getId();
    int  getTlc5947_id();
    bool hasPassedTargetKeyFrame();

    void setDebug(bool debug);

private:
    // calculates the current Speed
    void calculateCurrentSpeed();
    void checkAnimation(long elapsedTime);

    // currently the id used for the TLC5947
    int _ledId;
    int _tlc5947_id;

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
