#include "Animation.h"

bool keyFrameCompare (KeyFrame i,KeyFrame j) { return (i.getTimeMs()<j.getTimeMs()); }

Animation::Animation() :
_isSorted(true), _firstTargetFrameRead(false) {}

int Animation::numberOfKeyFrames(){
  return _keyFrames.size();
}

bool Animation::isAnimationFinished() {
  // need -- because end() is actually passed the last element
  vector<KeyFrame>::iterator lastKeyFrame = _keyFrames.end();
  --lastKeyFrame;
  return _currentKeyFrameIter == lastKeyFrame;
}

bool Animation::needsTargetFrameUpdate(long elapsedTime) {

  if (isAnimationFinished()){
    return false;
  }
  if (! _firstTargetFrameRead){
    return true;
  }

  double currentTargetTime = _currentKeyFrameIter->getTimeMs();
  return (currentTargetTime < elapsedTime);
}

vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  // need resorting
  if (!_isSorted){
    std::sort (_keyFrames.begin(), _keyFrames.end(), keyFrameCompare);
    _currentKeyFrameIter = _keyFrames.begin();
    _isSorted = true;
  }

  vector<KeyFrame> nextKeyFrames;

  while (needsTargetFrameUpdate(elapsedTime)){
    // for first frame the iterator is already pointing to the correct frame
    if (! _firstTargetFrameRead){
      _firstTargetFrameRead = true;
    }
    else {
      ++_currentKeyFrameIter;
    }
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
  printf("Animation contains %d frames. \nCurrent frame: ", _keyFrames.size());
  _currentKeyFrameIter->printKeyFrame();
  printf("\n");
}
