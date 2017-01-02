#include "KeyFrameStepper.h"

bool debug = false;
#define ARDBUFFER 80
#define MAX_SPEED 600

KeyFrameStepper::KeyFrameStepper(Adafruit_StepperMotor *motor,  AccelStepper &astepper, int id,
                                 vector<KeyFrame> keyFrame, int endStopPin, bool reverseDirection)
  : _motor(motor), _astepper(astepper), _keyFrame(keyFrame), _id(id),
    _endStopPin(endStopPin), _reverseDirection(reverseDirection), _numFrames(keyFrame.size())
{
  _startTime = 0;
  _animationActive = false;

  _currentFrameIdx = 0;
  _currentPosition = 0;
  _currentSpeed = 0.0;

  _calibrateSpeed = -400;

}

void KeyFrameStepper::start() {
  if (_numFrames > 0) {

    pinMode(_endStopPin, INPUT_PULLUP);

    // initialize key frames
    _currentFrameIdx = 0;
    _currentKeyFrame = _keyFrame[_currentFrameIdx];
    _previousKeyFrame = KeyFrame();

    // set speed and time
    //updateSpeed(0,0);  // not required but get strange compiler error if not there
    updateSpeed(0);  
    _startTime = millis();

    // allow animation to run
    _animationActive = true;

    serPrintln("%d ***START  Tgt t: %d t: %l", _id, _currentKeyFrame.getTimeMs(), getRuntime(), 0);
    Serial.println(getRuntime());
    serPrintln("%d Start  Tgt pos: %d Act: %l", _id, 0, getCurrentPosition(), 0);

  }
}
void KeyFrameStepper::init() {
  
  updateSpeed(0);
  _astepper.runSpeed();
}

void KeyFrameStepper::calibrate() {

  pinMode(_endStopPin, INPUT_PULLUP);
  
  serPrintln("%d ***CALIBRATING...", _id);

  // go up until end stop is hit
  updateSpeed(_calibrateSpeed);

  while (! isEndStopHit()) {
    _astepper.runSpeed();
  }

  serPrintln("%d ...reached end stop...", _id);
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
  //release();
  serPrintln("%d Calibrating finished.", _id);
}

void KeyFrameStepper::loop() {

  if (isEndStopHit()) {
    operateOnEndStop();
    _animationActive = false;
    return;
  }

  updateCurrentKeyFrame();

  if (_animationActive) {

    runStepper();
  }
}

void KeyFrameStepper::updateCurrentKeyFrame() {

  if (_animationActive) {
    long runtime = getRuntime();
    long currentTargetTime = _currentKeyFrame.getTimeMs();

    if (0) {
      Serial.print(_id);
      Serial.print(" **Update**");
      Serial.print("Runtime:");
      Serial.print(runtime);
      Serial.print(" TarTime:");
      Serial.print(currentTargetTime);
      Serial.print(" CurIdx:");
      Serial.print(_currentFrameIdx);
      Serial.print(" NumFrames:");
      Serial.println(_numFrames);
    }

    // update current key frame if required
    while (_currentKeyFrame.getTimeMs() <= runtime) {

      int curPos = (int) getCurrentPosition();
      int  tgtPos = _currentKeyFrame.getTarget();

      // we have passed the last key frame: end animation
      if (_currentFrameIdx == _numFrames - 1) {
        serPrintln("%d ***FINISH Exp t: %d t: %l", _id, currentTargetTime, runtime, 0);
        if (debug) Serial.println(runtime);
        serPrintln("%d Finish Exp pos: %d Act: %d", _id, tgtPos, curPos, 0);
        _animationActive = false;
        updateSpeed(0);
        break;
      }
      else {
        serPrint("%d UPDATE Exp t: %d t: ", _id, currentTargetTime );
        Serial.println(runtime);
        serPrintln("%d Update Exp pos: %d Act: %d", _id, tgtPos, curPos, 0);
        // read next key frame and update speed
        _currentFrameIdx++;
        _previousKeyFrame = _currentKeyFrame;
        _currentKeyFrame  = _keyFrame[_currentFrameIdx];
        updateSpeed(curPos, runtime);
      }
    }
  }
}

void KeyFrameStepper::updateSpeed(double speed) {

  if (speed != _currentSpeed) {
    if (abs(speed) > MAX_SPEED) {
      speed = speed > 0 ? MAX_SPEED : -MAX_SPEED;
    }
    _currentSpeed =  speed;
    double act_speed = _reverseDirection ? - _currentSpeed : _currentSpeed;
    // bug: 0 is not possible
    if (speed == 0){
      _astepper.setSpeed(1);
    }
    else {
      _astepper.setSpeed(act_speed);
    }
    serPrintln("%d Update Speed: %f Act: %f", _id, _currentSpeed, act_speed, 0);
  }
}

void KeyFrameStepper::updateSpeed(int curPos, long runTime) {
//  double newSpeed =  1000 * ((double)(_currentKeyFrame.getTarget() - _previousKeyFrame.getTarget()))
//                     / (_currentKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
  double newSpeed =  1000 * ((double)(_currentKeyFrame.getTarget() - curPos))
                     / (_currentKeyFrame.getTimeMs() - runTime);
  updateSpeed(newSpeed);
}

long KeyFrameStepper::getRuntime() {
  return millis() - _startTime;
}

long KeyFrameStepper::getCurrentPosition() {
  long curPos = _astepper.currentPosition();
  //serPrintln("%d Current Position: %d", _id, curPos);
  return _reverseDirection ? -curPos : curPos;
}

void KeyFrameStepper::resetPosition() {
  serPrintln("%d Reset Position", _id);
  _astepper.setCurrentPosition(0);
  long curPos = _astepper.currentPosition();
  serPrintln("%d Current Position: %d", _id, curPos, 0);
}

void KeyFrameStepper::release() {
  serPrintln("%d Release", _id);
  _motor->release();
}

bool KeyFrameStepper::isEndStopHit() {

  int endStop = digitalRead(_endStopPin);
  return (endStop == LOW);
}

void KeyFrameStepper::operateOnEndStop() {
  serPrintln("%d Endstop", _id);
  calibrate();
}

void KeyFrameStepper::runStepper() {
  _astepper.runSpeed();
}


