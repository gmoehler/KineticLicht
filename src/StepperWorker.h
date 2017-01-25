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

#include <math.h>
#include "KeyFrame.h"
#include "FiniteStates.h"

// State diagram:
// INIT -> CALIBRATING_UP ->(endstop is hit)-> CALIBRATING_ENDSTOPHIT[going downward] ->(endstop is released)-> CALIBRATION_FINISHED
// ACTIVE ->(endstop is hit)-> ENDSTOP_HIT[going downward] ->(300ms & endstop is released)-> AT_ENDSTOP_WAITING ->(speed is downward)-> ACTIVE
//        ->(time is past target time)-> PAST_TARGET ->(new target provided)-> ACTIVE

enum StepperWorkerState {INIT, CALIBRATING_UP, CALIBRATING_ENDSTOPHIT,
                         /*deprecated*/CALIBRATING , CALIBRATION_FINISHED,
                         ACTIVE,
                         ENDSTOP_WAITING, ENDSTOP_HIT,
                         PAST_TARGET,
                         NUM_STATES};

class StepperWorker : public FiniteStateMachine<StepperWorker>
{
  public:
    StepperWorker(AccelStepper &astepper, int id,
                  int endStopPin, bool reverseDirection);

    // to be called in setup()
    void init();

    // to be called in loop()
    void loop(long elapsedTime);

    // to be called in loop() during calibration
    // deprecated: use loop()
    void loopCalibration();

    // start calibration program
    void startCalibration();

    // sets internal state to ACTIVE and animates along the passed TargetKeyFrames
    void startAnimation();

    // set a new target keyFrame,
    // not handled as extra state since it only affects ACTIVE &&
    // AT_ENDSTOP_WAITING states
    void updateTargetKeyFrame(long elapsedTime, KeyFrame& kf);

    StepperWorkerState getState();

    void setDebug(bool debug);

  private:

    // did the end stop switch detect the light
    bool _to_endstop_hit();

    bool _to_endstop_waiting();

    void checkAnimation(long elapsedTime);

    // update to a specified speed
    void updateSpeed(double newSpeed);

    // calculate a new speed based on the next key frame
    void _updateSpeed(int curPos, long runTime);

    // where we actually do the stepping work
    void runStepper();

    // get current position of stepper
    long getCurrentPosition();

    // reset the current position to 0
    void resetPosition();

    // what to do when an end stop is reached (during calibration or at any other time)
    void operateOnEndStopHit();

    // state action on ENDSTOP_HIT
    void runOnEndStopHit();

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
    long _time_endstophit;
    bool _targetChanged;
    bool _triggerActive;
    long _elapsedTime;
    bool _debug;


};

#endif
