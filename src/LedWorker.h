//LedWorker.h

#ifndef LedWorker_h
#define LedWorker_h

#include <Arduino.h>
#include <ArduinoSTL.h>

#include "KeyFrame.h"
#include "RgbLED.h"
#include "RGB.h"

using namespace std;

class LedWorker
{
public:
    LedWorker(int rgbId);

    void loop(long elapsedTime);
    // update current key frames
    void updateTargetKeyFrame(long elapsedTime, KeyFrame& kf);
    bool needsUpdate();
    RGB  getColorForUpdate();
    int  getId();

    void setDebug(bool debug);

private:
    // calculates the current Speed
    void calculateCurrentSpeed(long elapsedTime);
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
