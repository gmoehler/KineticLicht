// StepperWorker.h

#ifndef StepperWorker_h
#define StepperWorker_h

#ifndef WITHIN_UNITTEST
  #include <Arduino.h>
  #include <AccelStepper.h>
  #include <avr/pgmspace.h>
#else
  #include <stdlib.h>
  #include <stdio.h>
  #include "../test/mock_Arduino.h"
#endif

#include <sstream>
#include <math.h>
#include <KineticLicht.h>
#include "KeyFrame.h"
#include "FiniteStates.h"

#define SW_DEBUG


// State diagram:
// INIT -> CALIBRATING_UP ->(endstop is hit)-> CALIBRATING_ENDSTOPHIT[going downward] ->(endstop is released)-> CALIBRATION_FINISHED
// ACTIVE ->(endstop is hit)-> ENDSTOP_HIT[going downward] ->(300ms & endstop is released)-> AT_ENDSTOP_WAITING ->(speed is downward)-> ACTIVE
//        ->(time is past target time)-> PAST_TARGET ->(new target provided)-> ACTIVE

enum StepperWorkerState {INIT,                    // 0
                         CALIBRATING_UP,          // 1
                         CALIBRATING_ENDSTOPHIT,  // 2
                         CALIBRATION_FINISHED,    // 3
                         ACTIVE,                  // 4
                         PAST_TARGET,             // 5
                         ENDSTOP_HIT,             // 6
                         ENDSTOP_WAITING,         // 7
                         NUM_STATES};             // only used for nbr of states

class StepperWorker : public FiniteStateMachine<StepperWorker>
{
  public:
    StepperWorker(uint8_t id, AccelStepper &astepper,
                  uint8_t endStopPin, bool reverseDirection);

    // to be called in setup()
    void init();

    // to be called in loop()
    void loop(long elapsedTime);

    // start calibration program
    void startCalibration();

    // sets internal state to ACTIVE and animates along the passed TargetKeyFrames
    void startAnimation();

    // set a new target keyFrame,
    // not handled as extra state since it only affects ACTIVE &&
    // AT_ENDSTOP_WAITING states
    void updateTargetKeyFrame(long elapsedTime, KeyFrame& kf);

    StepperWorkerState getState();

    uint8_t getId();

  private:
    // state functions
    void _entry_active();
    void _action_active();

    bool _to_endstop_hit();

    void _entry_endstop_hit();
    void _action_endstop_hit();
    void _exit_endstop_hit();

    bool _to_endstop_waiting();

    void _entry_endstop_waiting();
    void _action_endstop_waiting();

    bool _endstop_waiting_to_active();

    void _entry_calibrating_up();
    void _action_calibrating_up();

    void _entry_calibration_finished();
    void _action_calibration_finished();

    bool _to_past_target();

    void _entry_past_target();
    void _action_past_target();

    bool _past_target_to_active();

    // whether the endstop switch was pressed
    bool _endStopActive();

    // calculate speed based on current position and target
    double _calculateTargetSpeed();

    // update to a specified speed
    void _updateSpeed(double newSpeed);

    // get the current position from the accelstepper
    int _getCurrentPosition();

    uint8_t _id;               // stepper id
    AccelStepper& _astepper;

    double _currentSpeed;     // cached current speed

    uint8_t _endStopPin;          // pin of endstop switch

    bool _reverseDirection;   // direction counted in corect/reverse dir

    KeyFrame _targetKeyFrame;  // current target frame

    long _time_endstophit;    // time we hit the endstop
    bool _targetChanged;      // whether we got a new target
    long _elapsedTime;        // current elapsed time

};

#endif
