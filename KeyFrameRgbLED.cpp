#include "KeyFrameRgbLED.h"

bool debug1 = false;

KeyFrameRgbLED::KeyFrameRgbLED(int ledId, vector<KeyFrameRgb>& keyFrame)
  : _rgbLed(ledId), _keyFrame(keyFrame), _ledId(ledId), _numFrames(keyFrame.size())
{
  _startTime = 0;
  _animationActive = false;
  _needsUpdate = false;

  _currentFrameIdx = 0;
  _currentRedSpeed = 0.0;
  _currentGreenSpeed = 0.0;
  _currentBlueSpeed = 0.0;

  _needsUpdateDelta = 16;

}

int KeyFrameRgbLED::getId() {
  return _rgbLed.getId();
}

void KeyFrameRgbLED::start() {
  if (_numFrames > 0) {
    _animationActive = true;
    _startTime = millis();
    _currentKeyFrame = _keyFrame[_currentFrameIdx];
    _previousKeyFrame = KeyFrameRgb();
    calculateCurrentSpeed();

    if (debug1) {
      Serial.print("RGB Start time:");
      Serial.print(_startTime);
      Serial.print(" Speed: ");
      Serial.print(_currentRedSpeed);
      Serial.print (" ");
      Serial.print(_currentGreenSpeed);
      Serial.print(" ");
      Serial.println(_currentBlueSpeed);
    }
  }
}

void KeyFrameRgbLED::loop() {

  updateCurrentKeyFrame();

  if (_animationActive) {

    int delta = getRuntime() - _previousKeyFrame.getTimeMs();

    int expectedRed   = _previousKeyFrame.getRed() + _currentRedSpeed * delta;
    int expectedGreen = _previousKeyFrame.getGreen() + _currentGreenSpeed * delta;
    int expectedBlue  = _previousKeyFrame.getBlue() + _currentBlueSpeed * delta;
    int expectedBrightness  = _previousKeyFrame.getBrightness() + _currentBrightnessSpeed * delta;

    if (debug1) {
      Serial.print("***RGB Loop***");
      Serial.print("Time:");
      Serial.print(getRuntime());
      Serial.print(" RGB:");
      Serial.print(expectedRed);
      Serial.print(" ");
      Serial.print(expectedGreen);
      Serial.print(" ");
      Serial.println(expectedBlue);
    }

    RGB expectedColor = _rgbLed.getRGB(expectedRed, expectedGreen, expectedBlue, expectedBrightness);
    _needsUpdate = abs(expectedColor.red() - _currentColor.red()) > _needsUpdateDelta ||
                   abs(expectedColor.green() - _currentColor.green()) > _needsUpdateDelta ||
                   abs(expectedColor.blue() - _currentColor.blue()) > _needsUpdateDelta;
      serPrintln("###### CUR %d %d %d %d", _rgbLed.getId(), _currentColor.red(), _currentColor.green(), _currentColor.blue());
      serPrintln("###### EXP %d %d %d %d B: %d", _rgbLed.getId(), expectedColor.red(), expectedColor.green(), expectedColor.blue(), expectedBrightness);
    if (_needsUpdate) {
      _currentColor = expectedColor;
      serPrintln("###### UPDATE %d %d %d %d", _rgbLed.getId(), _currentColor.red(), _currentColor.green(), _currentColor.blue());
    }
  }
}

bool KeyFrameRgbLED::needsUpdate() {
  return _needsUpdate;
}

RGB KeyFrameRgbLED::getCurrentColor() {
  //serPrintln("****** %d %d %d %d", _rgbLed.getId(), _currentColor.red(), _currentColor.green(), _currentColor.blue());
  return _currentColor;
}

void KeyFrameRgbLED::updateCurrentKeyFrame() {

  if (_animationActive) {
    unsigned long runtime = getRuntime();
    //    Serial.println(runtime);

    unsigned long currentTargetTime = _currentKeyFrame.getTimeMs();

    if (debug1) {
      Serial.print(_ledId);
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
    if (runtime >= currentTargetTime) {
      if (debug1) {
        Serial.print("LED");
        Serial.print(_ledId);
        Serial.print(" RGB Update time:");
        Serial.println(runtime);
      }
      // after last key frame
      if (_currentFrameIdx == _numFrames - 1) {
        /*Serial.print(_ledId);
          Serial.print("RGB Finish time:");
          Serial.println(runtime);*/
        _animationActive = false;
        _currentColor = _rgbLed.black();
        _needsUpdate = true;
      }
      else {
        /*  Serial.print(_ledId);
          Serial.print(" RGB NextFrame: Speed ");*/
        _currentFrameIdx++;
        _previousKeyFrame = _currentKeyFrame;
        _currentKeyFrame  = _keyFrame[_currentFrameIdx];
        calculateCurrentSpeed();
        /* Serial.print(_currentRedSpeed);
          Serial.print (" ");
          Serial.print(_currentGreenSpeed);
          Serial.print(" ");
          Serial.println(_currentBlueSpeed);*/
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
  _currentBrightnessSpeed =  ((double)(_currentKeyFrame.getBrightness() - _previousKeyFrame.getBrightness()))
                       / (_currentKeyFrame.getTimeMs() - _previousKeyFrame.getTimeMs());
  serPrintln("LED%d Update Current Speed: %f, %f, %f, %f", _ledId, _currentRedSpeed, _currentGreenSpeed, _currentBlueSpeed, _currentBrightnessSpeed, 0);
}

unsigned long KeyFrameRgbLED::getRuntime() {
  return millis() - _startTime;
}

