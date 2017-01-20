// StepperWorker.h

#ifndef StepperWorker_h
#define StepperWorker_h

#ifndef WITHIN_UNITTEST
  #include <Arduino.h>
  #include <Adafruit_MotorShield.h>
  #include <AccelStepper.h>
#else
    #include <stdlib.h>
    #include <stdio.h>
    #include "../test/mock_Arduino.h"
#endif

#include "KeyFrame.h"

enum StepperWorkerState {INIT, CALIBRATING, CALIBRATION_FINISHED, ACTIVE, PAST_TARGET, ENDSTOP_HIT};

class StepperWorker
{
  public:
    StepperWorker(AccelStepper &astepper, int id,
                  int endStopPin, bool reverseDirection);

    // to be called in setup()
    void init();

    // to be called in loop()
    void loop(long elapsedTime);

    // to be called in loop() during calibration
    void loopCalibration();

    // set a new target keyFrame
    void updateTargetKeyFrame(long elapsedTime, KeyFrame& kf);

    StepperWorkerState getState();

    void setDebug(bool debug);

  private:

    // did the end stop switch detect the light
    bool isEndStopHit();

    void checkAnimation(long elapsedTime);

    // update to a specified speed
    void updateSpeed(double newSpeed);

    // calculate a new speed based on the next key frame
    void updateSpeed(int curPos, long runTime);

    // where we actually do the stepping work
    void runStepper();

    // get current position of stepper
    long getCurrentPosition();

    // reset the current position to 0
    void resetPosition();

    // what to do when an end stop is reached (during calibration or at any other time)
    void operateOnEndStopHit();

    AccelStepper& _astepper;

    int _id;

    StepperWorkerState _state;
    long _currentPosition;
    double _currentSpeed;

    int _endStopPin;
    bool _reverseDirection;
    int _calibrateSpeed;

    KeyFrame _previousKeyFrame;
    KeyFrame _targetKeyFrame;

    int _targetTimeDelta;
    bool _debug;
};

#endif
