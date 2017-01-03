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

void LedWorker::updateTargetKeyFrame(long elapsedTime, KeyFrame& kf) {

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
        Serial.print("!!! Passed KeyFrame:");
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

    RGB prevColor = _previousKeyFrame.getTargetColor();
    int delta = elapsedTime - _previousKeyFrame.getTimeMs();

    int expectedRed   = prevColor.red()   + _currentRedSpeed   * delta;
    int expectedGreen = prevColor.green() + _currentGreenSpeed * delta;
    int expectedBlue  = prevColor.blue()  + _currentBlueSpeed  * delta;

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
  RGB targetColor = _targetKeyFrame.getTargetColor();
  RGB prevColor = _previousKeyFrame.getTargetColor();
  
  _currentRedSpeed =  ((double)(targetColor.red() - prevColor.red()))
                      / (_targetKeyFrame.getTimeMs() - elapsedTime);
  _currentGreenSpeed =  ((double)(targetColor.green() - prevColor.green()))
                        / (_targetKeyFrame.getTimeMs() - elapsedTime);
  _currentBlueSpeed =  ((double)(targetColor.blue() - prevColor.blue()))
                       / (_targetKeyFrame.getTimeMs() - elapsedTime);

  serPrintln("LED%d Update Current Speed: %f, %f, %f", getId(), _currentRedSpeed, _currentGreenSpeed, _currentBlueSpeed, 0);
}


