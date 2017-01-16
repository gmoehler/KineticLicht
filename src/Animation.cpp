#include "Animation.h"

bool keyFrameCompare (KeyFrame i,KeyFrame j) { return (i.getTimeMs()<j.getTimeMs()); }

Animation::Animation() {}

int Animation::numberOfKeyFrames(){
  return _keyFrames.size();
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

  double nextTargetTime = nextKeyFrameIter->getTimeMs();
  return (nextTargetTime >= elapsedTime);
}

vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  // need resorting
/*  if (!_isSorted){
    std::sort (_keyFrames.begin(), _keyFrames.end(), keyFrameCompare);
    _currentKeyFrameIter = _keyFrames.begin();
    _isSorted = true;
  }
*/
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

  // trigger resorting
  _isSorted = false;
}

void Animation::printAnimation(){
    printf("Animation contains %d frames. Current frame: ", _keyFrames.size());
    _currentKeyFrameIter->printKeyFrame();
}
