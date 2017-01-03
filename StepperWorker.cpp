#include "StepperWorker.h"

#define ARDBUFFER 80
#define MAX_SPEED 600

StepperWorker::StepperWorker(Adafruit_StepperMotor *motor,  AccelStepper &astepper,
                             int id, int endStopPin, bool reverseDirection)
  : _motor(motor), _astepper(astepper), _id(id),
    _endStopPin(endStopPin), _reverseDirection(reverseDirection),
    _pastTargetKeyFrame(false), _currentPosition(0), _currentSpeed(0.0), _calibrateSpeed(400),
    _targetTimeDelta(250), _debug(false)
{ }

void StepperWorker::init() {
  pinMode(_endStopPin, INPUT_PULLUP);
}

void StepperWorker::updateTargetKeyFrame(long elapsedTime, KeyFrame& kf) {

  _previousKeyFrame = _targetKeyFrame;
  _targetKeyFrame  = kf;
  long curPos = _astepper.currentPosition();
  updateSpeed(curPos, elapsedTime);
  _pastTargetKeyFrame = false;
}

void StepperWorker::loop(long elapsedTime) {

  if (isEndStopHit()) {
    operateOnEndStopHit();
  }

  checkAnimation(elapsedTime);

  if (!_pastTargetKeyFrame) {
    runStepper();
  }
}


void StepperWorker::operateOnEndStopHit() {
  serPrintln("%d Endstop", _id);
  delay(300);

  // go down 200ms and then until end stop is released again
  serPrintln("%d ...going down - phase 1...", _id);
  updateSpeed(60);
  long now = millis();

  while (millis() - now < 200) {
    _astepper.runSpeed();
  }

  serPrintln("%d ...going down - phase 2...", _id);
  while (isEndStopHit()) {
    _astepper.runSpeed();
  }

  // call this 0
  resetPosition();
  _atEndStop = true;

  serPrintln("%d Reset finished.", _id);
}

void StepperWorker::resetPosition() {
  serPrintln("%d Reset Position", _id);
  _astepper.setCurrentPosition(0);
  long curPos = _astepper.currentPosition();
  serPrintln("%d Current Position: %d", _id, curPos, 0);
}

void StepperWorker::checkAnimation(long elapsedTime) {

  if (!_pastTargetKeyFrame) {
    unsigned long targetTime = _targetKeyFrame.getTimeMs();

    // did we run past the target key frame
    // allow for a bit of overshooting (_targetTimeDelta)
    if (elapsedTime >= targetTime + _targetTimeDelta) {

      int curPos = (int) getCurrentPosition();
      int tgtPos = _targetKeyFrame.getTarget();

      _pastTargetKeyFrame = true;
      // stop motor
      updateSpeed(0);

      if (_debug) {
        Serial.print("Passed KeyFrame:");
        Serial.print(_id);
        Serial.print(" Elapsed Time:");
        Serial.println(elapsedTime);
      }
      serPrintln("%d *** Exp t: %d t: %l", _id, targetTime, elapsedTime, 0);
      if (_debug) Serial.println(elapsedTime);
      serPrintln("%d     Exp pos: %d Act: %d", _id, tgtPos, curPos, 0);
    }
  }
}

void StepperWorker::calibrate() {

  pinMode(_endStopPin, INPUT_PULLUP);

  serPrintln("%d ***CALIBRATING...", _id);

  // go up until end stop is hit
  updateSpeed(_calibrateSpeed);

  while (! isEndStopHit()) {
    _astepper.runSpeed();
  }

  serPrintln("%d ...reached end stop...", _id);

  operateOnEndStopHit();
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
    serPrintln("%d Update Speed: %f Act: %f", _id, _currentSpeed, act_speed, 0);
  }
}

void StepperWorker::updateSpeed(int curPos, long elapsedTime) {
  //  double newSpeed =  1000 * ((double)(_targetKeyFrame.getTarget() - _previousKeyFrame.getTarget()))
  //                     / (_targetKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
  double newSpeed =  1000 * ((double)(_targetKeyFrame.getTarget() - curPos))
                     / (_targetKeyFrame.getTimeMs() - elapsedTime);
  updateSpeed(newSpeed);
}

long StepperWorker::getCurrentPosition() {
  long curPos = _astepper.currentPosition();
  //serPrintln("%d Current Position: %d", _id, curPos);
  return _reverseDirection ? -curPos : curPos;
}


void StepperWorker::release() {
  serPrintln("%d Release", _id);
  _motor->release();
}

bool StepperWorker::isEndStopHit() {

  int endStop = digitalRead(_endStopPin);
  return (endStop == LOW);
}


void StepperWorker::runStepper() {
  _astepper.runSpeed();
}


