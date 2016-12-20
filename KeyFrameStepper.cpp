#include "KeyFrameStepper.h"

bool debug = false;

KeyFrameStepper::KeyFrameStepper(Adafruit_StepperMotor *motor,  AccelStepper &astepper, int id, KeyFrame keyFrame[], int numFrames, int endStopPin)
  : _motor(motor), _astepper(astepper), _keyFrame(keyFrame), _numFrames(numFrames), _id(id), _endStopPin(endStopPin)
{
  _startTime = 0;
  _animationActive = false;

  _currentFrameIdx = 0;
  _currentPosition = 0;
  _currentSpeed = 0.0;

}

void KeyFrameStepper::start() {
  if (_numFrames > 0) {

    pinMode(_endStopPin, INPUT_PULLUP);

    // calibrate position
    calibrate();

    // initialize key frames
    _currentFrameIdx = 0;
    _currentKeyFrame = _keyFrame[_currentFrameIdx];
    _previousKeyFrame = KeyFrame();

    // set speed and time
    updateSpeed();
    _startTime = millis();

    // allow animation to run
    _animationActive = true;

    Serial.print(_id);
    Serial.print(" Start time:");
    Serial.print(_startTime);
    Serial.print(" Speed:");
    Serial.println(_currentSpeed);
  }
}

void KeyFrameStepper::calibrate() {

  serprint0("Calibrating...");

  // go up until end stop is hit
  _astepper.setSpeed(-20);

  while (! isEndStopHit()) {
    _astepper.runSpeed();
  }

  Serial.print("...reached end stop...");
  delay(100);

  // go down until end stop is released again
  _astepper.setSpeed(20);
  while (isEndStopHit()) {
    _astepper.runSpeed();
  }

  // call this 0
  resetPosition();
  //release();
  Serial.println (" ...finished.");
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

    /*
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
    */
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

      // we have passed the last key frame: end animation
      if (_currentFrameIdx == _numFrames - 1) {
        Serial.print(_id);
        Serial.print(" Finish time:");
        Serial.println(runtime);
        _animationActive = false;
        updateSpeed(0);
        break;
      }
      else {
        serprint("%d Expected position: %d Actual: %l", _id, _currentKeyFrame.getTarget(), getCurrentPosition());

        // read next key frame and update speed
        _currentFrameIdx++;
        _previousKeyFrame = _currentKeyFrame;
        _currentKeyFrame  = _keyFrame[_currentFrameIdx];
        updateSpeed();
      }
    }
  }
}

void KeyFrameStepper::updateSpeed(double speed) {

  if (speed != _currentSpeed) {
    _currentSpeed =  speed;

    _astepper.setSpeed(_currentSpeed);

    serprint("Update Speed: %f", _currentSpeed);
  }
}

void KeyFrameStepper::updateSpeed() {
  double newSpeed =  1000 * ((double)(_currentKeyFrame.getTarget() - _previousKeyFrame.getTarget()))
                     / (_currentKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
  updateSpeed(newSpeed);
}

unsigned long KeyFrameStepper::getRuntime() {
  return millis() - _startTime;
}

long KeyFrameStepper::getCurrentPosition() {
  serprint("%d Current Position", _id);
  return _astepper.currentPosition();
}

void KeyFrameStepper::resetPosition() {
  serprint("%d Reset Position", _id);
  _astepper.setCurrentPosition(0);
}

void KeyFrameStepper::release() {
  serprint("%d Release", _id);
  _motor->release();
}

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void KeyFrameStepper::forwardStep() {
  _motor->onestep(FORWARD, INTERLEAVE);
}
void KeyFrameStepper::backwardStep() {
  _motor->onestep(BACKWARD, INTERLEAVE);
}

bool KeyFrameStepper::isEndStopHit() {

  int endStop = digitalRead(_endStopPin);
  return (endStop == LOW);
}

void KeyFrameStepper::operateOnEndStop() {
  calibrate();
}

void KeyFrameStepper::runStepper() {
  _astepper.runSpeed();
}

void KeyFrameStepper::serprint0(char* str) {

  Serial.print(_id);
  Serial.print(" ");
  Serial.print(str);
}

void KeyFrameStepper::serprint1(char* str) {

  Serial.print(_id);
  Serial.print(" ");
  Serial.println(str);
}

int KeyFrameStepper::serprint(char* str, ...) {

  int i, j, count = 0;

  va_list argv;
  va_start(argv, str);
  for (i = 0, j = 0; str[i] != '\0'; i++) {
    if (str[i] == '%') {
      count++;

      Serial.write(reinterpret_cast<const uint8_t*>(str + j), i - j);

      switch (str[++i]) {
        case 'd': Serial.print(va_arg(argv, int));
          break;
        case 'l': Serial.print(va_arg(argv, long));
          break;
        case 'f': Serial.print(va_arg(argv, double));
          break;
        case 'c': Serial.print((char) va_arg(argv, int));
          break;
        case 's': Serial.print(va_arg(argv, char *));
          break;
        case '%': Serial.print("%");
          break;
        default:;
      };

      j = i + 1;
    }
  };
  va_end(argv);

  if (i > j) {
    Serial.write(reinterpret_cast<const uint8_t*>(str + j), i - j);
  }

  return count;
}

