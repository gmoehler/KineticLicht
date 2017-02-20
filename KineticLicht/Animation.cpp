#include "Animation.h"

bool keyFrameCompare (KeyFrame i,KeyFrame j) { return (i.getTimeMs()<j.getTimeMs()); }

Animation::Animation() :
   _currentFrameId(0), _isSorted(true), _firstTargetFrameRead(false), _withMotor(false) {}

Animation::Animation(vector<KeyFrame> kfs): Animation() {
  addKeyFrames(kfs);
}

Animation::Animation(const Animation &a) :
  _keyFrames(a._keyFrames), _currentFrameId(a._currentFrameId),
   _isSorted(a._isSorted), _firstTargetFrameRead(a._firstTargetFrameRead),
   _withMotor(a._withMotor){}

Animation::~Animation(){}

unsigned Animation::numberOfKeyFrames(){
  return _keyFrames.size();
}

 bool Animation::containsMotorFrames(){
 	return _withMotor;
 }

bool Animation::isAnimationFinished() {
  // need -- because end() is actually passed the last element
  /*vector<KeyFrame>::iterator lastKeyFrame = _keyFrames.end();
  --lastKeyFrame;
  return _currentKeyFrameIter == lastKeyFrame;*/
  return numberOfKeyFrames() == 0 || _currentFrameId >= numberOfKeyFrames();
}

bool Animation::needsTargetFrameUpdate(long elapsedTime) {

//  if (isAnimationFinished()){
//    return false;
//  }
  if (! _firstTargetFrameRead){
    return true;
  }

  double currentTargetTime = _keyFrames[_currentFrameId].getTimeMs();//_currentKeyFrameIter->getTimeMs();
  return (currentTargetTime < elapsedTime);
}

vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  // need resorting
  if (!_isSorted){
    std::sort (_keyFrames.begin(), _keyFrames.end(), keyFrameCompare);
    //_currentKeyFrameIter = _keyFrames.begin();
    _currentFrameId = 0;
    _isSorted = true;
  }

  vector<KeyFrame> nextKeyFrames;

  while (!isAnimationFinished() && needsTargetFrameUpdate(elapsedTime)){
    // for first frame the iterator is already pointing to the correct frame
    if (! _firstTargetFrameRead){
      _firstTargetFrameRead = true;
    }
    else {
      ++_currentFrameId;
    }
    nextKeyFrames.push_back(_keyFrames[_currentFrameId]);
  }

  return nextKeyFrames;
}

void Animation::addKeyFrames(vector<KeyFrame> kfs) {

  bool emptyFrameList = _keyFrames.size() == 0;

  // need loop to account for _withMotor instead of...
  // _keyFrames.insert(_keyFrames.end(), kfs.begin(), kfs.end());
  for (std::vector<KeyFrame>::iterator it = kfs.begin() ; it != kfs.end(); ++it) {
    _keyFrames.push_back(*it);
    if (it->getType() == MOTOR) {
    	_withMotor = true;
    }
  }

  // set iterator for the first time
  if (emptyFrameList){
    resetCurrentKeyFrame();
  }

  // trigger resorting
  _isSorted = false;
}

void Animation::resetCurrentKeyFrame(){
  if (_keyFrames.size() > 0){
    //_currentKeyFrameIter = _keyFrames.begin();
    _currentFrameId = 0;
  }
}

void Animation::printAnimation(){
  printf("Animation contains %d frames. \n Current frame: ", _keyFrames.size());
  if (_keyFrames.size() > 0){
    //_currentKeyFrameIter->printKeyFrame();
    _keyFrames[_currentFrameId].printKeyFrame();
  }
  printf("\n");
}

  KeyFrame& Animation::getKeyFrame(unsigned i){
    return _keyFrames[i];
  }
