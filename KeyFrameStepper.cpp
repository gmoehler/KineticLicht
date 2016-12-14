#include "KeyFrameStepper.h"

bool debug = false;

KeyFrameStepper::KeyFrameStepper(Adafruit_StepperMotor *motor, int id, KeyFrame keyFrame[], int numFrames)
 : _motor(motor), _keyFrame(keyFrame), _numFrames(numFrames), _id(id)
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

    Serial.print(_id);
    Serial.print(" Start time:");
    Serial.print(_startTime);
    Serial.print(" Speed:");
    Serial.println(_currentSpeed);
  }
}

void KeyFrameStepper::loop() {

  updateCurrentKeyFrame();

  if (_animationActive) {

    unsigned long time = getRuntime();

    unsigned long expectedPosition = _previousKeyFrame.getTarget() + _currentSpeed * (time -  _previousKeyFrame.getTimeMs());

    if (debug) {
      Serial.print(_id);
      Serial.print(" ***Loop***");
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
      Serial.print(_id);
      Serial.print(" Update time:");
      Serial.println(runtime);
      if (_currentFrameIdx == _numFrames - 1) {
        Serial.print(_id);
        Serial.print(" Finish time:");
        Serial.println(runtime);
        _animationActive = false;
        release();
        break;
      }
      else {
        Serial.print(_id);
        Serial.print(" NextFrame: Speed ");
        _currentFrameIdx++;
        _previousKeyFrame = _currentKeyFrame;
        _currentKeyFrame  = _keyFrame[_currentFrameIdx];
        calculateCurrentSpeed();
        Serial.println(_currentSpeed);
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
void KeyFrameStepper::forwardStep() {
  _motor->onestep(FORWARD, INTERLEAVE);
}
void KeyFrameStepper::backwardStep() {
  _motor->onestep(BACKWARD, INTERLEAVE);
}
void KeyFrameStepper::release() {
  Serial.println("Release");
  _motor->release();
}

