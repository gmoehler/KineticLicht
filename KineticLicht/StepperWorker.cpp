#include "StepperWorker.h"

#define ARDBUFFER 80
// max speed SINGLE: ca. 220 (with one light: down:350, up: 230)
// max speed INTERLEAVE: ca. 350
#define MAX_SPEED 200
#define CALIBRATE_SPEED -200
#define ALLOWED_TARGETTIME_OVERSHOOT 100

StepperWorker::StepperWorker(uint8_t id, AccelStepper &astepper,
  uint8_t endStopPin, bool reverseDirection)
  : _currentState(INIT), _prevState(INIT),
  _id(id), _astepper(astepper),
  _currentSpeed(0.0), _endStopPin(endStopPin),
  _reverseDirection(reverseDirection),
  _targetKeyFrame(KeyFrame()),
  _time_endstophit (0), _targetChanged (false), _elapsedTime(0)
  {
    /*
    std::stringstream sstr;
    sstr << "StepperWorker-" << _id;
    setDebugString(sstr.str());

    addStateEntryAction(ACTIVE, &StepperWorker::_entry_active);
    addStateAction(ACTIVE,  &StepperWorker::_action_active);

    addTransition (ACTIVE, ENDSTOP_HIT, &StepperWorker::_to_endstop_hit);

    addStateEntryAction(ENDSTOP_HIT, &StepperWorker::_entry_endstop_hit);
    addStateAction(ENDSTOP_HIT, &StepperWorker::_action_endstop_hit);
    addStateAction(ENDSTOP_HIT, &StepperWorker::_exit_endstop_hit);

    addTransition(ENDSTOP_HIT, ENDSTOP_WAITING, &StepperWorker::_to_endstop_waiting);

    addStateEntryAction(ENDSTOP_WAITING, &StepperWorker::_entry_endstop_waiting);
    addStateAction(ENDSTOP_WAITING, &StepperWorker::_action_endstop_waiting);

    addTransition(ENDSTOP_WAITING, ACTIVE, &StepperWorker::_endstop_waiting_to_active);

    addStateEntryAction(CALIBRATING_UP, &StepperWorker::_entry_calibrating_up);
    addStateAction(CALIBRATING_UP, &StepperWorker::_action_calibrating_up);

    // same as for normal endstop hit
    addTransition (CALIBRATING_UP, CALIBRATING_ENDSTOPHIT, &StepperWorker::_to_endstop_hit);

    addStateEntryAction(CALIBRATING_ENDSTOPHIT, &StepperWorker::_entry_endstop_hit);
    addStateAction(CALIBRATING_ENDSTOPHIT, &StepperWorker::_action_endstop_hit);
    addStateExitAction(CALIBRATING_ENDSTOPHIT, &StepperWorker::_exit_endstop_hit);

    addTransition(CALIBRATING_ENDSTOPHIT, CALIBRATION_FINISHED, &StepperWorker::_to_endstop_waiting);

    addStateEntryAction(CALIBRATION_FINISHED, &StepperWorker::_entry_calibration_finished);
    addStateAction(CALIBRATION_FINISHED, &StepperWorker::_action_calibration_finished);

    addTransition(ACTIVE, PAST_TARGET, &StepperWorker::_to_past_target);

    addStateEntryAction(PAST_TARGET, &StepperWorker::_entry_past_target);
    addStateAction(PAST_TARGET, &StepperWorker::_action_past_target);

    addTransition(PAST_TARGET, ACTIVE, &StepperWorker::_past_target_to_active);
    */
  }


  void StepperWorker::init() {
    pinMode(_endStopPin, INPUT_PULLUP);
  }

  void StepperWorker::startAnimation(){
    //TODO: think about calling exit funtions of current state
    _prevState = _currentState;
    _currentState = ACTIVE;
  }

  uint8_t StepperWorker::getId(){
    return _id;
  }

  void StepperWorker::_entry_active(){
    FPRINTF1(sw_msg0, "STP %d: Entering state active\n", _id);
    double newSpeed = _calculateTargetSpeed();
    _updateSpeed(newSpeed);
  }

  void StepperWorker::_action_active(){
    if (_targetChanged){
      #ifdef SW_DEBUG
      FPRINTF1(sw_msg1, "STP %d: Target changed. ", _id);
      #endif
      double newSpeed = _calculateTargetSpeed();
      _updateSpeed(newSpeed);
    }
    _astepper.runSpeed();
  }


  bool StepperWorker::_to_endstop_hit() {
    return _endStopActive();
  }

  bool StepperWorker::_endStopActive() {
    int endStop = digitalRead(_endStopPin);
    if (endStop == LOW){
      FPRINTF2(sw_msg2, "STP %d: Endstop hit: %d\n", _id, endStop);
    }
    return (endStop == LOW);
  }

  void StepperWorker::_entry_endstop_hit(){
    FPRINTF1(sw_msg3, "STP: %d Entering state endstop_hit\n", _id);
    _updateSpeed(60);
    _time_endstophit = _elapsedTime;
  }

  void StepperWorker::_action_endstop_hit(){
    _astepper.runSpeed();
  }

  void StepperWorker::_exit_endstop_hit(){
    #ifdef SW_DEBUG
    long curPos = _getCurrentPosition();
    FPRINTF2(sw_msg4, "STP %d: Reset position, act: %ld\n", _id, curPos);
    #endif
    _astepper.setCurrentPosition(0);
  }

  // when endstop is not pressed down anymore and more then 300 ms passed
  bool StepperWorker::_to_endstop_waiting() {
    return (_elapsedTime - _time_endstophit) > 300 && ! _endStopActive();
  }

  void StepperWorker::_entry_endstop_waiting(){
    FPRINTF1(sw_msg5, "STP %d: Entering state endstop_waiting\n", _id);
    _updateSpeed(0);
    _astepper.runSpeed(); // required?
    _time_endstophit = 0;
  }

  void StepperWorker::_action_endstop_waiting() {
    // nothing to be done
  }

  bool StepperWorker::_endstop_waiting_to_active() {
    // continue when it goes down again
    if (_targetChanged && _calculateTargetSpeed() > 0.0){
      _targetChanged = false;
      return true;
    }
    return false;
  }

  void StepperWorker::startCalibration(){
    //TODO triggerTransition(getState(), CALIBRATING_UP);
  }

  void StepperWorker::_entry_calibrating_up(){
    FPRINTF1(sw_msg6, "STP %d: Entering state entry_calibrating\n", _id);
    _updateSpeed(CALIBRATE_SPEED);
  }

  void StepperWorker::_action_calibrating_up(){
    _astepper.runSpeed();
  }

  void StepperWorker::_entry_calibration_finished(){
    FPRINTF1(sw_msg7, "STP %d: Entering state calibration_finished\n", _id);
    _updateSpeed(0);
    _astepper.runSpeed(); // required?
  }

  void StepperWorker::_action_calibration_finished(){
    // just wait
  }

  double StepperWorker::_calculateTargetSpeed() {
    long curPos = _getCurrentPosition();
    //  double newSpeed =  1000 * ((double)(_targetKeyFrame.getTarget() - _previousKeyFrame.getTarget()))
    //                     / (_targetKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
    double deltaTime = _targetKeyFrame.getTimeMs() - _elapsedTime;
    return deltaTime != 0 ? 1000 * ((double)(_targetKeyFrame.getTarget() - curPos)) / deltaTime : 0;
  }


  void StepperWorker::_updateSpeed(double speed) {
    if (speed != _currentSpeed) {
      if (fabs(speed) > MAX_SPEED) {
        speed = speed > 0 ? MAX_SPEED : -MAX_SPEED;
      }
      _currentSpeed =  speed;

      double act_speed = _reverseDirection ? - _currentSpeed : _currentSpeed;
      // bug: 0 is not possible
      if (speed == 0.0) {
        _astepper.setSpeed(1); // downward to be on the safe side
      }
      else {
        _astepper.setSpeed(act_speed);
      }

      #ifdef SW_DEBUG
      FPRINTF2(sw_msg8, "STP %d: Set speed: %d\n", _id, (int) _currentSpeed);
      /*      int curSpeed = 1000 * _currentSpeed;
      int actSpeed = 1000 * act_speed;
      FPRINTF3(sw_msg9, "%d Update Speed to %d Act: %d\n", _id, curSpeed, actSpeed);*/
      #endif
    }
    // when speed was set then new target was used in anyway
    _targetChanged = false;
  }

  void StepperWorker::updateTargetKeyFrame(long elapsedTime, KeyFrame& kf) {
    #ifdef SW_DEBUG
    kf.printNewKeyFrame("STP");
    #endif
    _targetKeyFrame  = kf;
    _targetChanged = true;
  }

  StepperWorkerState StepperWorker::getState(){
    return _currentState;
  }

  bool StepperWorker::_to_past_target(){
    long targetTime = _targetKeyFrame.getTimeMs();
    return _elapsedTime >= targetTime + ALLOWED_TARGETTIME_OVERSHOOT;
  }

  void StepperWorker::_entry_past_target(){
    _updateSpeed(0);
    _astepper.runSpeed(); // required?

    #ifdef SW_DEBUG
    int tgtPos = _targetKeyFrame.getTarget();
    long tgtTime = _targetKeyFrame.getTimeMs();
    FPRINTF1(sw_msg12, "STP %d: Passed KeyFrame ", _id);
    _targetKeyFrame.printKeyFrame();
    FPRINTF3(sw_msg13, "STP %d Tgt t  : %ld Act t  : %ld\n", _id, tgtTime, _elapsedTime);
    FPRINTF3(sw_msg14, "STP %d Tgt pos: %d Act pos: %d\n", _id, tgtPos, _getCurrentPosition());
    #endif
  }

  void StepperWorker::_action_past_target(){
    // just wait
  }

  bool StepperWorker::_past_target_to_active(){
    // continue with next target when we received one
    if (_targetChanged){
      _targetChanged = false;
      return true;
    }
    return false;
  }

  int StepperWorker::_getCurrentPosition() {
    long curPos = _astepper.currentPosition();
    //FPRINTF2(sw_msg15, "STP %d: Current Position: %d", _id, curPos);
    return (int) (_reverseDirection ? -curPos : curPos);
  }

  void StepperWorker::loop(long elapsedTime) {
    _elapsedTime = elapsedTime;
    StepperWorkerState nextState = _currentState;

    // transitions (if no transition was triggered before)
    if (_prevState == _currentState){
      switch (_currentState) {
        case INIT:
        // transition triggered elsewhere
        break;

        case CALIBRATING_UP:
        if (_to_endstop_hit()){
          nextState = ENDSTOP_HIT;
        }
        break;

        case CALIBRATING_ENDSTOPHIT:
        if (_to_endstop_waiting()){
          nextState = ENDSTOP_WAITING;
        }
        break;

        case CALIBRATION_FINISHED:
        // transition triggered elsewhere
        break;

        case ACTIVE:
        if (_to_endstop_hit()){
          nextState = ENDSTOP_HIT;
        }
        else if (_to_past_target()){
          nextState = PAST_TARGET;
        }
        break;

        case PAST_TARGET:
        if (_past_target_to_active()){
          nextState = ACTIVE;
        }
        break;

        case ENDSTOP_HIT:
        if (_to_endstop_waiting()){
          nextState = ENDSTOP_WAITING;
        }
        break;

        case ENDSTOP_WAITING:
        if (_endstop_waiting_to_active()){
          nextState = ACTIVE;
        }
        break;

        default:
        // no other valid state
        break;
      }
    }


    // exit functions
    if (_prevState != _currentState){
      switch (_currentState) {
        case INIT:
        // no exit function
        break;

        case CALIBRATING_UP:
        // no exit function
        break;

        case CALIBRATING_ENDSTOPHIT:
        _exit_endstop_hit();
        break;

        case CALIBRATION_FINISHED:
        // no exit function
        break;

        case ACTIVE:
        // no exit function
        break;

        case PAST_TARGET:
        // no exit function
        break;

        case ENDSTOP_HIT:
        _exit_endstop_hit();
        break;

        case ENDSTOP_WAITING:
        // no exit function
        break;

        default:
        // no other valid state
        break;
      }
    }

    // entry functions and actions
    switch (_currentState) {
      case INIT:
      // no action
      break;

      case CALIBRATING_UP:
      if (_prevState != _currentState){
        _entry_calibrating_up();
      }
      _action_calibrating_up();
      break;

      case CALIBRATING_ENDSTOPHIT:
      if (_prevState != _currentState){
        _entry_endstop_hit();
      }
      _action_endstop_hit();
      break;

      case CALIBRATION_FINISHED:
      if (_prevState != _currentState){
        _entry_calibration_finished();
      }
      _action_calibration_finished();
      break;

      case ACTIVE:
      if (_prevState != _currentState){
        _entry_active();
      }
      _action_active();
      break;

      case PAST_TARGET:
      if (_prevState != _currentState){
        _entry_past_target();
      }
      _action_past_target();
      break;

      case ENDSTOP_HIT:
      if (_prevState != _currentState){
        _entry_endstop_hit();
      }
      _action_endstop_hit();
      break;

      case ENDSTOP_WAITING:
      if (_prevState != _currentState){
        _entry_endstop_waiting();
      }
      _action_endstop_waiting();
      break;

      default:
      // no other valid state
      break;
    }

    // update states
    _prevState = _currentState;
    _currentState = nextState;
  }
