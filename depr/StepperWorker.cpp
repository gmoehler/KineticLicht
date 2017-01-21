#include "StepperWorker.h"

#define ARDBUFFER 80
#define MAX_SPEED 600

StepperWorker::StepperWorker(AccelStepper &astepper,
                             int id, int endStopPin, bool reverseDirection)
  :  _astepper(astepper), _id(id),
    _state(INIT), _currentPosition(0), _currentSpeed(0.0),
    _endStopPin(endStopPin), _reverseDirection(reverseDirection), _calibrateSpeed(-400),
    _previousKeyFrame(KeyFrame()), _targetTimeDelta(250), _debug(true)
{}

void StepperWorker::init() {
  pinMode(_endStopPin, INPUT_PULLUP);
}

void StepperWorker::setDebug(bool debug){
  _debug = debug;
}

StepperWorkerState StepperWorker::getState(){
  return _state;
}

void StepperWorker::updateTargetKeyFrame(long elapsedTime, KeyFrame& kf) {

  if(_debug){
    printf("%d: New Key frame:\n", _id);
    kf.printKeyFrame();
  }
  _previousKeyFrame = _targetKeyFrame;
  _targetKeyFrame  = kf;
  long curPos = _astepper.currentPosition();
  updateSpeed(curPos, elapsedTime);
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

  runStepper();
}


void StepperWorker::loop(long elapsedTime) {

  if (isEndStopHit()) {
    _state = ENDSTOP_HIT;
    operateOnEndStopHit();
  }

  checkAnimation(elapsedTime);

  if (_state == ACTIVE || // already active
     (_state == ENDSTOP_HIT && _currentSpeed > 0) || // going down again after endstop hit
     (_state == CALIBRATION_FINISHED) || // calibration finished
      _state == INIT  // we have skipped calibration (e.g. in unit test)
     ) {
    _state = ACTIVE;
    runStepper();
  }
}


void StepperWorker::operateOnEndStopHit() {
  printf("%d Endstop\n", _id);
  delay(300);

  // go down 200ms
  printf("%d ...going down - phase 1...\n", _id);
  updateSpeed(60);
  long now = millis();

  while (millis() - now < 200) {
    _astepper.runSpeed();
  }

  // then go down until end stop is released
  printf("%d ...going down - phase 2...\n", _id);
  while (isEndStopHit()) {
    _astepper.runSpeed();
  }

  // call this position 0
  resetPosition();
}

void StepperWorker::resetPosition() {
  printf("%d Reset Position\n", _id);
  _astepper.setCurrentPosition(0);
  long curPos = _astepper.currentPosition();
  printf("%d Current Position: %ld\n", _id, curPos);
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

void StepperWorker::updateSpeed(double speed) {

  if (speed != _currentSpeed) {
    if (abs(speed) > MAX_SPEED) {
      speed = speed > 0 ? MAX_SPEED : -MAX_SPEED;
    }
    _currentSpeed =  speed;
    double act_speed = _reverseDirection ? - _currentSpeed : _currentSpeed;
    // bug: 0 is not possible
    if (speed == 0) {
      _astepper.setSpeed(1);
    }
    else {
      _astepper.setSpeed(act_speed);
    }
    if (_debug){
      printf("%d Update Speed: %f Act: %f\n", _id, _currentSpeed, act_speed);
    }
  }
}

void StepperWorker::updateSpeed(int curPos, long elapsedTime) {
  //  double newSpeed =  1000 * ((double)(_targetKeyFrame.getTarget() - _previousKeyFrame.getTarget()))
  //                     / (_targetKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
  double deltaTime = _targetKeyFrame.getTimeMs() - elapsedTime;
  double newSpeed =  deltaTime > 0 ? 1000 * ((double)(_targetKeyFrame.getTarget() - curPos)) / deltaTime : 0;
  updateSpeed(newSpeed);
}

long StepperWorker::getCurrentPosition() {
  long curPos = _astepper.currentPosition();
  //serPrintln("%d Current Position: %d", _id, curPos);
  return _reverseDirection ? -curPos : curPos;
}

bool StepperWorker::isEndStopHit() {

  int endStop = digitalRead(_endStopPin);
  return (endStop == LOW);
}


void StepperWorker::runStepper() {
  _astepper.runSpeed();
}
