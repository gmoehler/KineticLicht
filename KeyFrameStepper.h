// KeyFrameStepper.h

#ifndef KeyFrameStepper_h
#define KeyFrameStepper_h

#include "Arduino.h"
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

#include "KeyFrame.h"

class KeyFrameStepper
{
  public:
    KeyFrameStepper(Adafruit_StepperMotor *motor, AccelStepper &astepper, int id, KeyFrame keyFrame[], int numFrames, int endStopPin);

    void start();
    void loop();
    bool isAnimationFinished();
    bool isEndStop();
    void calibrate();

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

    long getCurrentPosition();
    void resetPosition();
    void updateSpeed();

    Adafruit_StepperMotor *_motor;
    AccelStepper _astepper;
    
    int _id;
    KeyFrame* _keyFrame;
    int _numFrames;
    int _currentFrameIdx;
    long _currentPosition;
    double _currentSpeed;
    int _endStopPin;
    void serprint0(char* str);
    void serprint(char* str, ...);


    unsigned long _startTime;

    bool _animationActive;

    KeyFrame _previousKeyFrame;
    KeyFrame _currentKeyFrame;

};

#endif
