#include "StepperWorker.h"

#define ARDBUFFER 80
#define MAX_SPEED 600

StepperWorker::StepperWorker(AccelStepper &astepper,
                             int id, int endStopPin, bool reverseDirection)
  : FiniteStateMachine(NUM_STATES, INIT, *this),
    _astepper(astepper), _id(id),
    _state(INIT), _currentPosition(0), _currentSpeed(0.0),
    _endStopPin(endStopPin), _reverseDirection(reverseDirection), _calibrateSpeed(-400),
    _previousKeyFrame(KeyFrame()), _targetTimeDelta(250), _speed_updated (false),
    _debug(true)
{
  addStateAction(ACTIVE,  &StepperWorker::_action_active);

  // first transition function, should be called first (?)
  addTransition(ACTIVE, ENDSTOP_HIT, &StepperWorker::is_endstop_hit);
}

void StepperWorker::init() {
  pinMode(_endStopPin, INPUT_PULLUP);
}

void StepperWorker::loop(long elapsedTime) {
  _elapsedTime = elapsedTime;
  FiniteStateMachine::loop();
}

SteperWorker::_action_active(){
  if (_targetChanged){
    double newSpeed = calculateTargetSpeed();
    updateSpeed(newSpeed);
  }
  _astepper.runSpeed();
}

bool StepperWorker::_to_endstop_hit() {
  return _endStopActive();
}

bool StepperWorker::_endStopActive() {
  int endStop = digitalRead(_endStopPin);
  return (endStop == LOW);
}


StepperWorker::_entry_endstop_hit(){
  updateSpeed(60);
  _time_endstophit = millis();
}

StepperWorker::_action_endstop_hit(){
  _astepper.runSpeed();
}

StepperWorker::_exit_endstop_hit(){
  _astepper.setCurrentPosition(0);
  if (_debug) {
    long curPos = _astepper.currentPosition();
    printf("%d Current Position: %ld\n", _id, curPos);
  }
}

// when endstop is not pressed down anymore and more then 300 ms passed
bool StepperWorker::_to_endstop_waiting() {
    long now = millis();
    return (now - _time_endstophit) > 300 && ! _endStopActive();
}

StepperWorker::_entry_endstop_waiting(){
  updateSpeed(0);
  _astepper.runSpeed(); // need this only once to stop
  _time_endstophit = 0;
}

StepperWorker::_action_endstop_waiting {
	// nothing to be done
}

bool StepperWorker::_endstop_to_active() {
	// continue when it goes down again
    if (_targetChanged && calculateTargetSpeed() > 0.0){
      _targetChanged = false;
      return false;
    }
    return false;
}

void StepperWorker::_entry_active(){
  double newSpeed = calculateSpeed();
  updateSpeed(newSpeed);
}

bool StepperWorker::_init_to_active(){
	return _triggerActive
}

bool StepperWorker::_getResetTriggerActive {
	if (_triggerActive) {
		_triggerActive = false;
		return true;
	}
	

double StepperWorker::calculateTargetSpeed() {
  long curPos = _astepper.currentPosition();
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

    // bug: 0 is not possible
    if (speed == 0) {
      _astepper.setSpeed(1); // downward to be on the safe side
    }
    else {
      _astepper.setSpeed(act_speed);
    }

    double act_speed = _reverseDirection ? - _currentSpeed : _currentSpeed;

    if (_debug){
      printf("%d Update Speed: %f Act: %f\n", _id, _currentSpeed, act_speed);
    }
  }
}

void StepperWorker::updateTargetKeyFrame(long elapsedTime, KeyFrame& kf) {
  if(_debug){
    printf("%d: New Key frame:\n", _id);
    kf.printKeyFrame();
  }
  _previousKeyFrame = _targetKeyFrame;
  _targetKeyFrame  = kf;
  _targetChanged = true;
}

void StepperWorker::setDebug(bool debug){
  _debug = debug;
}

StepperWorkerState StepperWorker::getState(){
  return _state;
}

void StepperWorker::startCalibration(){
  // TODO: do we need a state transition instead?
  _state=CALIBRATING;
}

void StepperWorker::startAnimation(){
  _triggerActive = true;
}



void StepperWorker::loopCalibration() {

  if (isEndStopHit()) {
    operateOnEndStopHit();
    _state = CALIBRATION_FINISHED;
    return;
  }

  // on first call in each calibration cycle only
  if (_state != CALIBRATING){
    updateSpeed(_calibrateSpeed);
    _state = CALIBRATING;
  }

astepper.runSpeed();
}





void StepperWorker::checkAnimation(long elapsedTime) {

  if (_state == ACTIVE) {
    long targetTime = _targetKeyFrame.getTimeMs();

    // did we run past the target key frame
    // allow for a bit of overshooting (_targetTimeDelta)
    if (elapsedTime >= targetTime + _targetTimeDelta) {

      int curPos = (int) getCurrentPosition();
      int tgtPos = _targetKeyFrame.getTarget();

      _state = PAST_TARGET;
      // stop motor
      updateSpeed(0);

      if (_debug) {
        printf("!!! Passed KeyFrame: %d\n", _id);
        printf("%d *** Exp t:  %ld t:   %ld\n", _id, targetTime, elapsedTime);
        printf("%d     Exp pos: %d Act: %d\n", _id, tgtPos, curPos);
      }
    }
  }
}

long StepperWorker::getCurrentPosition() {
  long curPos = _astepper.currentPosition();
  //serPrintln("%d Current Position: %d", _id, curPos);
  return _reverseDirection ? -curPos : curPos;
}
