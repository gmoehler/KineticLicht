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
    void updateKeyFrame(KeyFrameRgb& kf, long elapsedTime);
    bool needsUpdate();
    RGB getCurrentColor();
    int getId();

protected:

private:

    // calculates the current Speed
    void calculateCurrentSpeed(long elapsedTime);
    void checkAnimation(long elapsedTime);

    int _ledId;
    
    RGB    _currentColor;
    double _currentRedSpeed;
    double _currentGreenSpeed;
    double _currentBlueSpeed;

    bool _pastKeyFrame;
    bool _needsUpdate;

    int _needsUpdateDelta;
    bool _debug;

    KeyFrameRgb _previousKeyFrame;
    KeyFrameRgb _currentKeyFrame;
     
};

#endif 
