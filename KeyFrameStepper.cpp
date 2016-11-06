#include "KeyFrameStepper.h"

bool debug = false;

KeyFrameStepper::KeyFrameStepper(AF_Stepper motor, KeyFrame keyFrame[], int numFrames)
 : _motor(motor), _keyFrame(keyFrame), _numFrames(numFrames)
{
  _startTime = 0;
  _animationActive = false;

  _currentFrameIdx = 0;
  _currentPosition = 0;
  _currentSpeed = 0.0;
}

void KeyFrameStepper::start() {
  if (_numFrames > 0) {
    _animationActive = true;
    _startTime = millis();
    _currentKeyFrame = _keyFrame[_currentFrameIdx];
    _previousKeyFrame = KeyFrame();
    calculateCurrentSpeed();

    Serial.print("Start time:");
    Serial.println(_startTime);
  }
}

void KeyFrameStepper::loop() {

  if (_animationActive) {

    updateCurrentKeyFrame();

    unsigned long time = getRuntime();

    unsigned long expectedPosition = _previousKeyFrame.getTarget() + _currentSpeed * (time -  _previousKeyFrame.getTimeMs());

    if (debug) {
      Serial.print("***Loop***");
      Serial.print("Time:");
      Serial.print(time);
      Serial.print(" Speed:");
      Serial.print(_currentSpeed);
      Serial.print(" Pos:");
      Serial.print(_currentPosition);
      Serial.print(" ExpPos:");
      Serial.println(expectedPosition);
    }

    bool dirPrinted = false;
    while (_currentPosition < expectedPosition) {
      _currentPosition += 1;
      forwardStep();
      if (!dirPrinted) {
        //Serial.println("->forward");
        dirPrinted = true;
      }
    }
    dirPrinted = false;
    while (_currentPosition > expectedPosition) {
      _currentPosition -= 1;
      backwardStep();
      if (!dirPrinted) {
        // Serial.println("->backward");
        dirPrinted = true;
      }
    }
  }
}

void KeyFrameStepper::updateCurrentKeyFrame() {

  if (_animationActive) {
    unsigned long runtime = getRuntime();
//    Serial.println(runtime);

    unsigned long currentTargetTime = _currentKeyFrame.getTimeMs();

    if (debug) {
      Serial.print("**Update**");
      Serial.print("Runime:");
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
      Serial.print("Update time:");
      Serial.println(runtime);
      if (_currentFrameIdx == _numFrames - 1) {
        Serial.print("Finish time:");
        Serial.println(runtime);
        _animationActive = false;
        release();
        break;
      }
      else {
        Serial.println("NextFrame");
        _currentFrameIdx++;
        _previousKeyFrame = _currentKeyFrame;
        _currentKeyFrame  = _keyFrame[_currentFrameIdx];
        calculateCurrentSpeed();
      }
    }
  }
}

void KeyFrameStepper::calculateCurrentSpeed() {
  _currentSpeed =  ((double)(_currentKeyFrame.getTarget() - _previousKeyFrame.getTarget()))
                   / (_currentKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
}

unsigned long KeyFrameStepper::getRuntime() {
  return millis() - _startTime;
}

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
// wrappers for the first motor!
void KeyFrameStepper::forwardStep() {
  _motor.onestep(FORWARD, DOUBLE);
}
void KeyFrameStepper::backwardStep() {
  _motor.onestep(BACKWARD, DOUBLE);
}
void KeyFrameStepper::release() {
  Serial.println("Release");
  _motor.release();
}

