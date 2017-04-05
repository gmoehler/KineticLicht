//LedWorker.h

#ifndef LEDWORKER_H
#define LEDWORKER_H

#ifndef WITHIN_UNITTEST
  #include <Arduino.h>
  #include <ArduinoSTL.h>
#else
  #include <stdlib.h>
  #include <stdio.h>
  #include "../test/mock_Arduino.h"
#endif

#include "KineticLicht.h"
#include "KeyFrame.h"
#include "RGB.h"

#define LW_DEBUG

class LedWorker
{
public:
    LedWorker(uint8_t rgbId, uint8_t tlc5947_id);

    void init(); // to be called at setup()
    void loop(long elapsedTime); // to be called in loop();
    // update current key frames
    void updateTargetKeyFrame(long elapsedTime, KeyFrame& kf);
    bool needsUpdate();
    RGB  getColorForUpdate();
    uint8_t  getId();
    uint8_t  getTlc5947_id();
    bool hasPassedTargetKeyFrame();

private:
    // calculates the current Speed
    void calculateCurrentSpeed();
    void checkAnimation(long elapsedTime);

    uint8_t _ledId;
    uint8_t _tlc5947_id;

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

    KeyFrame _previousKeyFrame;
    KeyFrame _targetKeyFrame;

    // make copy constructor private
    // should not be used because it waists memory
    LedWorker (const LedWorker&);


};

#endif
