#include "LedWorker.h"

LedWorker::LedWorker(int ledId)
  : _ledId(ledId), _debug(false), _needsUpdate(false), 
    _currentRedSpeed(0.0), _currentGreenSpeed(0.0), _currentBlueSpeed(0.0),
    _needsUpdateDelta(16), _pastKeyFrame(false)
{ }

int LedWorker::getId() {
  return _ledId;
}

void LedWorker::updateKeyFrame(long elapsedTime, KeyFrameRgb& kf) {

    _previousKeyFrame = _currentKeyFrame;
    _currentKeyFrame  = kf;
    calculateCurrentSpeed(elapsedTime);
    _pastKeyFrame = false;

    if (_debug) {
     Serial.print("Update frame:");
      Serial.print(_ledId);
      Serial.print(" Speed: ");
      Serial.print(_currentRedSpeed);
      Serial.print (" ");
      Serial.print(_currentGreenSpeed);
      Serial.print(" ");
      Serial.println(_currentBlueSpeed);
    }
}

void LedWorker::checkAnimation(long elapsedTime) {

  if (!_pastKeyFrame) {
    unsigned long currentTargetTime = _currentKeyFrame.getTimeMs();

   // did we run past the key frame
    if (elapsedTime >= currentTargetTime) {
      if (_debug) {
        Serial.print("Passed KeyFrame:");
        Serial.print(getId());
        Serial.print(" Elapsed Time:");
        Serial.println(elapsedTime);
      }
      _pastKeyFrame = true;
      _needsUpdate = true;
    }
  }
}


void LedWorker::loop(long elapsedTime) {

  checkAnimation(elapsedTime);

  if (!_pastKeyFrame) {

    int delta = elapsedTime - _previousKeyFrame.getTimeMs();

    int expectedRed   = _previousKeyFrame.getRed() + _currentRedSpeed * delta;
    int expectedGreen = _previousKeyFrame.getGreen() + _currentGreenSpeed * delta;
    int expectedBlue  = _previousKeyFrame.getBlue() + _currentBlueSpeed * delta;

    if (_debug) {
      Serial.print("***RGB Loop***");
      Serial.print("Time:");
      Serial.print(elapsedTime);
      Serial.print(" RGB:");
      Serial.print(expectedRed);
      Serial.print(" ");
      Serial.print(expectedGreen);
      Serial.print(" ");
      Serial.println(expectedBlue);
    }

    RGB expectedColor = RGB(expectedRed, expectedGreen, expectedBlue, expectedBrightness);
    _needsUpdate = abs(expectedColor.red() - _currentColor.red()) > _needsUpdateDelta ||
                   abs(expectedColor.green() - _currentColor.green()) > _needsUpdateDelta ||
                   abs(expectedColor.blue() - _currentColor.blue()) > _needsUpdateDelta;
      serPrintln("###### CUR %d %d %d %d", getId(), _currentColor.red(), _currentColor.green(), _currentColor.blue());
      serPrintln("###### EXP %d %d %d %d B: %d", getId(), expectedColor.red(), expectedColor.green(), expectedColor.blue(), expectedBrightness);
    if (_needsUpdate) {
      _currentColor = expectedColor;
      serPrintln("###### UPDATE %d %d %d %d", getId(), _currentColor.red(), _currentColor.green(), _currentColor.blue());
    }
  }
}

bool LedWorker::needsUpdate() {
  return _needsUpdate;
}

RGB LedWorker::getCurrentColor() {
  //serPrintln("****** %d %d %d %d", _rgbLed.getId(), _currentColor.red(), _currentColor.green(), _currentColor.blue());
  return _currentColor;
}


void LedWorker::calculateCurrentSpeed() {
  _currentRedSpeed =  ((double)(_currentKeyFrame.getRed() - _previousKeyFrame.getRed()))
                      / (_currentKeyFrame.getTimeMs() - elapsedTime);
  _currentGreenSpeed =  ((double)(_currentKeyFrame.getGreen() - _previousKeyFrame.getGreen()))
                        / (_currentKeyFrame.getTimeMs() - elapsedTime);
  _currentBlueSpeed =  ((double)(_currentKeyFrame.getBlue() - _previousKeyFrame.getBlue()))
                       / (_currentKeyFrame.getTimeMs() - elapsedTime);
  serPrintln("LED%d Update Current Speed: %f, %f, %f, %f", getId(), _currentRedSpeed, _currentGreenSpeed, _currentBlueSpeed, _currentBrightnessSpeed, 0);
}


