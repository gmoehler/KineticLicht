#include "KeyFrameStepper.h"

bool debug = true;
#define ARDBUFFER 80

KeyFrameStepper::KeyFrameStepper(Adafruit_StepperMotor *motor,  AccelStepper &astepper, int id,
                                 KeyFrame keyFrame[], int numFrames, int endStopPin, bool reverseDirection)
  : _motor(motor), _astepper(astepper), _keyFrame(keyFrame), _numFrames(numFrames), _id(id),
    _endStopPin(endStopPin), _reverseDirection(reverseDirection)
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

    serprintln("%d Start  Exp t: %d t: %d", _id, _currentKeyFrame.getTimeMs(), getRuntime());
    serprintln("%d Start  Exp pos: %d Act: %l", _id, 0, getCurrentPosition());
  }
}

void KeyFrameStepper::calibrate() {

  serprintln("Calibrating...");

  // go up until end stop is hit
  updateSpeed(-20);

  while (! isEndStopHit()) {
    _astepper.runSpeed();
  }

  Serial.println("...reached end stop...");
  delay(100);

  // go down until end stop is released again
  updateSpeed(20);
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
    unsigned long currentTargetTime = _currentKeyFrame.getTimeMs();

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
        serprintln("%d Finish Exp t: %d t: %d", _id, currentTargetTime, runtime);
        serprintln("%d Finish Exp pos: %d Act: %d", _id, tgtPos, curPos);
        _animationActive = false;
        updateSpeed(0);
        break;
      }
      else {
        serprintln("%d Update Exp t: %d t: %d", _id, currentTargetTime, runtime);
        serprintln("%d Update Exp pos: %d Act: %d", _id, tgtPos, curPos);

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
    double act_speed = _reverseDirection ? - _currentSpeed : _currentSpeed;
    _astepper.setSpeed(act_speed);
    serprintln("%d Update Speed: %f Act: %f", _id, _currentSpeed, act_speed);
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
  long curPos = _astepper.currentPosition();
  serprintln("%d Current Position: %d", _id, curPos);
  return curPos;
}

void KeyFrameStepper::resetPosition() {
  serprintln("%d Reset Position", _id);
  _astepper.setCurrentPosition(0);
  long curPos = _astepper.currentPosition();
  serprintln("%d Current Position: %d", _id, curPos);
}

void KeyFrameStepper::release() {
  serprintln("%d Release", _id);
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

void KeyFrameStepper::serprintln(char* str, ...) {
  if (debug) {
    int i, count = 0, j = 0, flag = 0;
    char temp[ARDBUFFER + 1];
    for (i = 0; str[i] != '\0'; i++)  if (str[i] == '%')  count++;

    va_list argv;
    va_start(argv, count);
    for (i = 0, j = 0; str[i] != '\0'; i++)
    {
      if (str[i] == '%')
      {
        temp[j] = '\0';
        Serial.print(temp);
        j = 0;
        temp[0] = '\0';

        switch (str[++i])
        {
          case 'd': Serial.print(va_arg(argv, int));
            break;
          case 'l': Serial.print(va_arg(argv, long));
            break;
          case 'f': Serial.print(va_arg(argv, double));
            break;
          case 'c': Serial.print((char)va_arg(argv, int));
            break;
          case 's': Serial.print(va_arg(argv, char *));
            break;
          default:  ;
        };
      }
      else
      {
        temp[j] = str[i];
        j = (j + 1) % ARDBUFFER;
        if (j == 0)
        {
          temp[ARDBUFFER] = '\0';
          Serial.print(temp);
          temp[0] = '\0';
        }
      }
    };
    Serial.println();

  /*
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
    Serial.println();
  */
}
}

