// KeyFrameRgbLED.h

#ifndef LedWorker_h
#define LedWorker_h

#include <Arduino.h>
#include <ArduinoSTL.h>

#include "Helpers.h"
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
    void updateTargetKeyFrame(long elapsedTime, KeyFrameRgb& kf);
    bool needsUpdate();
    RGB getColorForUpdate();
    int getId();

protected:

private:

    // calculates the current Speed
    void calculateCurrentSpeed(long elapsedTime);
    void checkAnimation(long elapsedTime);

    int _ledId;

    // actual color on the LED
    RGB    _currentColor;
    // expected color on the LED
    RGB    _expectedColor;

    double _currentRedSpeed;
    double _currentGreenSpeed;
    double _currentBlueSpeed;

    bool _pastTargetKeyFrame;
    bool _needsUpdate;

    int _needsUpdateDelta;
    bool _debug;

    KeyFrameRgb _previousKeyFrame;
    KeyFrameRgb _targetKeyFrame;
     
};

#endif 
