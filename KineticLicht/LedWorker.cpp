// LedWorker.cpp
#include "LedWorker.h"

LedWorker::LedWorker(uint8_t ledId, uint8_t tlc5947_id)
: _ledId(ledId), _tlc5947_id(tlc5947_id),
_currentRedSpeed(0.0), _currentGreenSpeed(0.0), _currentBlueSpeed(0.0),
_needsUpdateDelta(16), _pastTargetKeyFrame(false), _needsUpdate(false)
{ }

uint8_t LedWorker::getId() {
  return _ledId;
}

uint8_t LedWorker::getTlc5947_id() {
  return _tlc5947_id;
}

void LedWorker::init(){
  // do nothing for now
}

void LedWorker::updateTargetKeyFrame(long elapsedTime, KeyFrame& kf) {

#ifdef LW_DEBUG
    FPRINTF2(lw_msg0, "LED%d %ld New Key frame: \n", _ledId, elapsedTime);
    kf.printKeyFrame();
#endif

  _previousKeyFrame = _targetKeyFrame;
  _targetKeyFrame  = kf;
  calculateCurrentSpeed();
  _pastTargetKeyFrame = false;

#ifdef LW_DEBUG
    FPRINTF5(lw_msg1, "LED%d %ld Update frame Speed: %d %d %d\n", getId(), elapsedTime,
      (int) (1000*_currentRedSpeed),  (int) (1000*_currentGreenSpeed),  (int) (1000*_currentBlueSpeed));
#endif
}

void LedWorker::checkAnimation(long elapsedTime) {

  if (!_pastTargetKeyFrame) {
    long targetTime = _targetKeyFrame.getTimeMs();

    // did we run past the target key frame
    if (elapsedTime > targetTime) {
  #ifdef LW_DEBUG
        FPRINTF3(lw_msg2, "Warning. Passed KeyFrame: LED%d, Target time: %ld, Elapsed time: %ld\n", getId(), targetTime, elapsedTime);
  #endif
      _pastTargetKeyFrame = true;
      _needsUpdate = true;
    }
  }
}

bool LedWorker::hasPassedTargetKeyFrame(){
    return _pastTargetKeyFrame;
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
#ifdef LW_DEBUG
      FPRINTF5(lw_msg3, "LED%d %ld CUR %d %d %d\n", getId(), elapsedTime, _currentColor.red(), _currentColor.green(), _currentColor.blue());
      FPRINTF5(lw_msg4, "LED%d %ld EXP %d %d %d\n", getId(), elapsedTime, _expectedColor.red(), _expectedColor.green(), _expectedColor.blue());
#endif
  }
}

bool LedWorker::needsUpdate() {
  return _needsUpdate;
}

RGB LedWorker::getColorForUpdate() {
  // now that the color is realized we can set current Color
  _currentColor = _expectedColor;
  _needsUpdate = false;
#ifdef LW_DEBUG
    FPRINTF4(lw_msg5, "LED%d Read color %d %d %d\n", _ledId, _currentColor.red(), _currentColor.green(), _currentColor.blue());
#endif
  return _expectedColor;
}


void LedWorker::calculateCurrentSpeed() {
  RGB targetColor = _targetKeyFrame.getTargetColor();
  RGB prevColor = _previousKeyFrame.getTargetColor();
  long targetTime = _targetKeyFrame.getTimeMs();
  long prevTime = _previousKeyFrame.getTimeMs();

  _currentRedSpeed =  ((double)(targetColor.red() - prevColor.red()))
    / (targetTime - prevTime);
  _currentGreenSpeed =  ((double)(targetColor.green() - prevColor.green()))
    / (targetTime - prevTime);
  _currentBlueSpeed =  ((double)(targetColor.blue() - prevColor.blue()))
    / (targetTime - prevTime);
#ifdef LW_DEBUG
    FPRINTF4(lw_msg6, "LED%d Update Current Speed: %d, %d, %d\n", getId(),
    (int) (1000*_currentRedSpeed), (int) (1000*_currentGreenSpeed), (int)(1000*_currentBlueSpeed));
#endif
}
