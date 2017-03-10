#include "StepperWorker.h"

#define ARDBUFFER 80
#define MAX_SPEED 220
#define CALIBRATE_SPEED -200
#define ALLOWED_TARGETTIME_OVERSHOOT 100
// max speed INTERLEAVE (interl): ca. 350
// max speed INTERLEAVE (single): ca. 200

StepperWorker::StepperWorker(int8_t id, AccelStepper &astepper,
                             int8_t endStopPin, bool reverseDirection)
  : FiniteStateMachine(NUM_STATES, INIT, *this),
    _id(id), _astepper(astepper),
    _currentSpeed(0.0), _endStopPin(endStopPin),
    _reverseDirection(reverseDirection),
  //  _previousKeyFrame(KeyFrame()),
    _targetKeyFrame(KeyFrame()),
    _time_endstophit (0), _targetChanged (false), _elapsedTime(0),
    _debug(true)
{
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

}

void StepperWorker::init() {
  pinMode(_endStopPin, INPUT_PULLUP);
}

void StepperWorker::loop(long elapsedTime) {
  _elapsedTime = elapsedTime;
  FiniteStateMachine::loop();
}

void StepperWorker::startAnimation(){
  triggerTransition(getState(), ACTIVE);
}

int8_t StepperWorker::getId(){
  return _id;
}

void StepperWorker::_entry_active(){
  printf("%d Entering state active\n", _id);
  double newSpeed = _calculateTargetSpeed();
  _updateSpeed(newSpeed);
}

void StepperWorker::_action_active(){
  if (_targetChanged){
    if (_debug){
      printf("Target changed. Calc new speed. ");
    }
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
    printf("%d Endstop hit: %d\n", _id, endStop);
  }
  return (endStop == LOW);
}

void StepperWorker::_entry_endstop_hit(){
  printf("%d Entering state endstop_hit\n", _id);
  _updateSpeed(60);
  _time_endstophit = _elapsedTime;
}

void StepperWorker::_action_endstop_hit(){
  _astepper.runSpeed();
}

void StepperWorker::_exit_endstop_hit(){
  if (_debug) {
    long curPos = _getCurrentPosition();
    printf("%d Reset position, act: %ld\n", _id, curPos);
  }
  _astepper.setCurrentPosition(0);
}

// when endstop is not pressed down anymore and more then 300 ms passed
bool StepperWorker::_to_endstop_waiting() {
    return (_elapsedTime - _time_endstophit) > 300 && ! _endStopActive();
}

void StepperWorker::_entry_endstop_waiting(){
  printf("%d Entering state endstop_waiting\n", _id);
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
    triggerTransition(getState(), CALIBRATING_UP);
}

void StepperWorker::_entry_calibrating_up(){
  printf("%d Entering state entry_calibrating\n", _id);
  _updateSpeed(CALIBRATE_SPEED);
}

void StepperWorker::_action_calibrating_up(){
  _astepper.runSpeed();
}

void StepperWorker::_entry_calibration_finished(){
  printf("%d Entering state calibration_finished\n", _id);
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

    if (_debug){
      printf("%d Set speed: %d\n", _id, (int) _currentSpeed);
/*      int curSpeed = 1000 * _currentSpeed;
      int actSpeed = 1000 * act_speed;
      printf("%d Update Speed to %d Act: %d\n", _id, curSpeed, actSpeed);*/
    }
  }
  // when speed was set then new target was used in anyway
  _targetChanged = false;
}

void StepperWorker::updateTargetKeyFrame(long elapsedTime, KeyFrame& kf) {
  if(_debug){
    printf("%d: New Key frame:\n", _id);
    kf.printKeyFrame();
  }
//  _previousKeyFrame = _targetKeyFrame;
  _targetKeyFrame  = kf;
  _targetChanged = true;
}

void StepperWorker::setDebug(bool debug){
  _debug = debug;
}

StepperWorkerState StepperWorker::getState(){
  return StepperWorkerState(FiniteStateMachine::getState());
}

bool StepperWorker::_to_past_target(){
  long targetTime = _targetKeyFrame.getTimeMs();
  return _elapsedTime >= targetTime + ALLOWED_TARGETTIME_OVERSHOOT;
}

void StepperWorker::_entry_past_target(){
  _updateSpeed(0);
  _astepper.runSpeed(); // required?

  if (_debug) {
    int tgtPos = _targetKeyFrame.getTarget();
    long tgtTime = _targetKeyFrame.getTimeMs();
    printf("Warning. Passed KeyFrame for %d\n", _id);
    _targetKeyFrame.printKeyFrame();
    printf("%d *** Tgt t  : %ld Act t  : %ld\n", _id, tgtTime, _elapsedTime);
    printf("%d     Tgt pos: %d Act pos: %d\n", _id, tgtPos, _getCurrentPosition());
  }
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
  //serPrintln("%d Current Position: %d", _id, curPos);
  return (int) (_reverseDirection ? -curPos : curPos);
}
