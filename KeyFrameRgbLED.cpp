#include "KeyFrameRgbLED.h"

bool debug1 = false;

KeyFrameRgbLED::KeyFrameRgbLED(Adafruit_TLC5947* tlc, int ledId, KeyFrameRgb keyFrame[], int numFrames)
 : _rgbLed(tlc, ledId), _keyFrame(keyFrame), _numFrames(numFrames)
{
  _startTime = 0;
  _animationActive = false;

  _currentFrameIdx = 0;
  _currentRedSpeed = 0.0;
  _currentGreenSpeed = 0.0;
  _currentBlueSpeed = 0.0;

}

void KeyFrameRgbLED::start() {
  if (_numFrames > 0) {
    _animationActive = true;
    _startTime = millis();
    _currentKeyFrame = _keyFrame[_currentFrameIdx];
    _previousKeyFrame = KeyFrameRgb();
    calculateCurrentSpeed();

    Serial.print("RGB Start time:");
    Serial.println(_startTime);
  }
}

void KeyFrameRgbLED::loop() {

  if (_animationActive) {

    updateCurrentKeyFrame();

    unsigned long time = getRuntime();

    unsigned int expectedRed   = _previousKeyFrame.getRed() + _currentRedSpeed * (int) (time -  _previousKeyFrame.getTimeMs());
    unsigned int expectedGreen = _previousKeyFrame.getGreen() + _currentGreenSpeed * (int) (time -  _previousKeyFrame.getTimeMs());
    unsigned int expectedBlue  = _previousKeyFrame.getBlue() + _currentBlueSpeed * (int) (time -  _previousKeyFrame.getTimeMs());

    if (debug1) {
      Serial.print("***RGB Loop***");
      Serial.print("Time:");
      Serial.print(time);
      Serial.print(" RGB:");
      Serial.print(expectedRed);
      Serial.print(expectedGreen);
      Serial.println(expectedBlue);
    }

    _rgbLed.rgbOutput(expectedRed, expectedGreen, expectedBlue);
  }
}

void KeyFrameRgbLED::updateCurrentKeyFrame() {

  if (_animationActive) {
    unsigned long runtime = getRuntime();
//    Serial.println(runtime);

    unsigned long currentTargetTime = _currentKeyFrame.getTimeMs();

    if (debug1) {
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
      Serial.print("RGB Update time:");
      Serial.println(runtime);
      if (_currentFrameIdx == _numFrames - 1) {
        Serial.print("RGB Finish time:");
        Serial.println(runtime);
        _animationActive = false;
        _rgbLed.black();
        break;
      }
      else {
        Serial.println("RGB NextFrame");
        _currentFrameIdx++;
        _previousKeyFrame = _currentKeyFrame;
        _currentKeyFrame  = _keyFrame[_currentFrameIdx];
        calculateCurrentSpeed();
      }
    }
  }
}

void KeyFrameRgbLED::calculateCurrentSpeed() {
  _currentRedSpeed =  ((double)(_currentKeyFrame.getRed() - _previousKeyFrame.getRed()))
                   / (_currentKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
  _currentGreenSpeed =  ((double)(_currentKeyFrame.getGreen() - _previousKeyFrame.getGreen()))
                   / (_currentKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
  _currentBlueSpeed =  ((double)(_currentKeyFrame.getBlue() - _previousKeyFrame.getBlue()))
                   / (_currentKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
}

unsigned long KeyFrameRgbLED::getRuntime() {
  return millis() - _startTime;
}

