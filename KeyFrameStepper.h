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
    KeyFrameStepper(Adafruit_StepperMotor *motor, AccelStepper &astepper, int id, KeyFrame keyFrame[], 
                    int numFrames, int endStopPin, bool reverseDirection);

    // to be called in setup()
    void start();

    // to be called in loop()
    void loop();

    // did we pass the last key frame
    bool isAnimationFinished();

    // did the end stop switch detect the light
    bool isEndStopHit();

    // initial routine: tear up lights until they hit the end stop - call this 0 and go down a bit
    void calibrate();

  protected:

  private:
    // current time from start time to now
    long getRuntime();

    // update current and previous key frames
    void updateCurrentKeyFrame();

    // update to a specified speed
    void updateSpeed(double newSpeed);

    // calculate a new speed based on the next key frame
    void updateSpeed();

    // where we actually do the stepping work
    void runStepper();

    // one step forward
    void forwardStep();

    // one step backward
    void backwardStep();

    // release the motor
    void release();

    // get current position of stepper
    long getCurrentPosition();

    // reset the current position to 0
    void resetPosition();

    // what to do when an end stop is reached
    void operateOnEndStop();

    Adafruit_StepperMotor *_motor;
    AccelStepper _astepper;
    
    int _id;
    KeyFrame* _keyFrame;
    int _numFrames;
    int _currentFrameIdx;
    long _currentPosition;
    double _currentSpeed;
    int _endStopPin;
    bool _reverseDirection;
    void serprintln(char* str, ...);


    long _startTime;

    bool _animationActive;

    KeyFrame _previousKeyFrame;
    KeyFrame _currentKeyFrame;

};

#endif
