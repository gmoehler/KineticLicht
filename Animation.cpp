#include "Animation.h"

Animation::Animation() {
  // nothing to be done
}

bool Animation::isAnimationFinished() {
  return _currentKeyFrameIter == _keyFrames.end();
}
bool Animation::hasNextTargetKeyFrame(long elapsedTime) {

  if (isAnimationFinished()){
    return false;
  }

  // peak at next element
  vector<KeyFrame>::iterator nextKeyFrameIter = _currentKeyFrameIter;
  ++nextKeyFrameIter;

  return (nextKeyFrameIter->getTimeMs() <= elapsedTime);
}

vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  vector<KeyFrame> nextKeyFrames;

  while (hasNextTargetKeyFrame(elapsedTime)){
    ++_currentKeyFrameIter;
    nextKeyFrames.push_back(*_currentKeyFrameIter);
  }

  return nextKeyFrames;
}

void Animation::addKeyFrames(vector<KeyFrame> kfs) {

  bool firstKeyFramesAdded = _keyFrames.size() == 0 && kfs.count() > 0;

  _keyFrames.insert(_keyFrames.end(), kfs.begin(), kfs.end());

  if (firstKeyFramesAdded){
    _currentKeyFrameIter = _keyFrames.begin();
  }
}



vector<KeyFrame> Animation::getMotorKeyframes(int motorId) {
  vector<KeyFrame> kfs = keyFrames[motorId];
  return kfs;
}

void Animation::addMotorKeyFrame(int motorId, KeyFrame new_kf) {
  vector<KeyFrame> kfs = keyFrames[motorId];
  kfs.push_back(new_kf);
}

void Animation::addMotorKeyFrames(int motorId, vector<KeyFrame> new_kfs) {
  vector<KeyFrame> kfs = keyFrames[motorId];
  kfs.insert(kfs.end(), new_kfs.begin(), new_kfs.end());
}

void Animation::addMotorKeyFrames(vector<KeyFrame> id_kfs) {

  for (vector<KeyFrame>::iterator it = id_kfs.begin(); it != id_kfs.end(); ++it)
  {
    int motorId =  it->getId();
    addMotorKeyFrame(motorId, *it);
  }
}

vector<KeyFrameRgb> Animation::getRgbKeyframes(int ledId, LedPosition topOrBottom) {
  int index = getLedIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  return kfs;
}

void Animation::addRgbKeyFrame(int ledId, KeyFrameRgb new_kf, LedPosition topOrBottom) {
  int index = getLedIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  kfs.push_back(new_kf);
}

void Animation::addRgbKeyFrames(int ledId, vector<KeyFrameRgb> new_kfs, LedPosition topOrBottom) {
  int index = getLedIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  kfs.insert(kfs.end(), new_kfs.begin(), new_kfs.end());
}

int Animation::getLedIndex(int ledId, LedPosition topOrBottom) {
  int index = 2 * (ledId - 1) + 1;
  if (topOrBottom == TOP) {
    index ++;
  }
  return index;
}

