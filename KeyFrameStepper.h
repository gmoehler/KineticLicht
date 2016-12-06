// KeyFrameStepper.h

#ifndef KeyFrameStepper_h
#define KeyFrameStepper_h

#include "Arduino.h"
#include <Adafruit_MotorShield.h>
#include "KeyFrame.h"

class KeyFrameStepper
{
  public:
    KeyFrameStepper(Adafruit_StepperMotor *motor, KeyFrame keyFrame[], int numFrames);

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

    // one step forward
    void forwardStep();

    // one step backward
    void backwardStep();

    // release the motor
    void release();

    Adafruit_StepperMotor *_motor;
    KeyFrame* _keyFrame;
    int _numFrames;
    int _currentFrameIdx;
    long _currentPosition;
    double _currentSpeed;

    unsigned long _startTime;

    bool _animationActive;

    KeyFrame _previousKeyFrame;
    KeyFrame _currentKeyFrame;

};

#endif
