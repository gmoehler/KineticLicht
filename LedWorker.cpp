#include "LedWorker.h"

LedWorker::LedWorker(int ledId)
  : _ledId(ledId),
    _currentRedSpeed(0.0), _currentGreenSpeed(0.0), _currentBlueSpeed(0.0),
    _needsUpdateDelta(16), _pastTargetKeyFrame(false), _needsUpdate(false),
    _debug(false)
{ }

int LedWorker::getId() {
  return _ledId;
}

void LedWorker::updateTargetKeyFrame(long elapsedTime, KeyFrameRgb& kf) {

    _previousKeyFrame = _targetKeyFrame;
    _targetKeyFrame  = kf;
    calculateCurrentSpeed(elapsedTime);
    _pastTargetKeyFrame = false;

    if (_debug) {
     Serial.print("Update frame ");
      Serial.print(getId());
      Serial.print(" Speed: ");
      Serial.print(_currentRedSpeed);
      Serial.print (" ");
      Serial.print(_currentGreenSpeed);
      Serial.print(" ");
      Serial.println(_currentBlueSpeed);
    }
}

void LedWorker::checkAnimation(long elapsedTime) {

  if (!_pastTargetKeyFrame) {
    unsigned long targetTime = _targetKeyFrame.getTimeMs();

   // did we run past the target key frame
    if (elapsedTime >= targetTime) {
      if (_debug) {
        Serial.print("Passed KeyFrame:");
        Serial.print(getId());
        Serial.print(" Elapsed Time:");
        Serial.println(elapsedTime);
      }
      _pastTargetKeyFrame = true;
      _needsUpdate = true;
    }
  }
}


void LedWorker::loop(long elapsedTime) {

  checkAnimation(elapsedTime);

  if (!_pastTargetKeyFrame) {

    int delta = elapsedTime - _previousKeyFrame.getTimeMs();

    int expectedRed   = _previousKeyFrame.getRed()   + _currentRedSpeed   * delta;
    int expectedGreen = _previousKeyFrame.getGreen() + _currentGreenSpeed * delta;
    int expectedBlue  = _previousKeyFrame.getBlue()  + _currentBlueSpeed  * delta;

    _expectedColor = RGB(expectedRed, expectedGreen, expectedBlue);

    _needsUpdate = abs(_expectedColor.red()   - _currentColor.red())   > _needsUpdateDelta ||
                   abs(_expectedColor.green() - _currentColor.green()) > _needsUpdateDelta ||
                   abs(_expectedColor.blue()  - _currentColor.blue())  > _needsUpdateDelta;

    serPrintln("###### %l CUR %d %d %d %d", elapsedTime, getId(), _currentColor.red(), _currentColor.green(), _currentColor.blue());
    serPrintln("###### %l EXP %d %d %d %d", elapsedTime, getId(), _expectedColor.red(), _expectedColor.green(), _expectedColor.blue());
  }
}

bool LedWorker::needsUpdate() {
  return _needsUpdate;
}

RGB LedWorker::getColorForUpdate() {
    // now that the color is realized we can set current Color
    _currentColor = _expectedColor;
    _needsUpdate = false;
  //serPrintln("****** %d %d %d %d", _rgbLed.getId(), _currentColor.red(), _currentColor.green(), _currentColor.blue());
  return _expectedColor;
}


void LedWorker::calculateCurrentSpeed(long elapsedTime) {
  _currentRedSpeed =  ((double)(_targetKeyFrame.getRed() - _previousKeyFrame.getRed()))
                      / (_targetKeyFrame.getTimeMs() - elapsedTime);
  _currentGreenSpeed =  ((double)(_targetKeyFrame.getGreen() - _previousKeyFrame.getGreen()))
                        / (_targetKeyFrame.getTimeMs() - elapsedTime);
  _currentBlueSpeed =  ((double)(_targetKeyFrame.getBlue() - _previousKeyFrame.getBlue()))
                       / (_targetKeyFrame.getTimeMs() - elapsedTime);

  serPrintln("LED%d Update Current Speed: %f, %f, %f", getId(), _currentRedSpeed, _currentGreenSpeed, _currentBlueSpeed, 0);
}


