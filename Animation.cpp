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

  bool firstKeyFramesAdded = _keyFrames.size() == 0 && kfs.size() > 0;

  _keyFrames.insert(_keyFrames.end(), kfs.begin(), kfs.end());

  if (firstKeyFramesAdded){
    _currentKeyFrameIter = _keyFrames.begin();
  }
}
