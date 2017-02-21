#include "Animation.h"

bool keyFrameCompare (KeyFrame i,KeyFrame j) { return (i.getTimeMs()<j.getTimeMs()); }

Animation::Animation() :
   _currentFrameId(-1), _isSorted(true), _firstTargetFrameRead(false), _withMotor(false) {}

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
  return numberOfKeyFrames() == 0 || _currentFrameId >= (int) numberOfKeyFrames() -1;
}

bool Animation::needsTargetFrameUpdate(long elapsedTime) {

//  if (isAnimationFinished()){
//    return false;
//  }
  // nothing read so far
  if (_currentFrameId < 0){
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
    if (_keyFrames.size() > 0){
      _currentFrameId = -1;
      _isSorted = true;
    }
  }

  vector<KeyFrame> nextKeyFrames;

  while (!isAnimationFinished() && needsTargetFrameUpdate(elapsedTime)){
    // for first frame the iterator is already pointing to the correct frame
    _currentFrameId++;
    nextKeyFrames.push_back(_keyFrames[_currentFrameId]);
  }

  return nextKeyFrames;
}

void Animation::addKeyFrames(vector<KeyFrame> kfs) {

  // need loop to account for _withMotor instead of one-line insert
  // _keyFrames.insert(_keyFrames.end(), kfs.begin(), kfs.end());
  for (std::vector<KeyFrame>::iterator it = kfs.begin() ; it != kfs.end(); ++it) {
    _keyFrames.push_back(*it);
    if (it->getType() == MOTOR) {
    	_withMotor = true;
    }
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
  printf("Animation contains %d frames. \n", _keyFrames.size());
  if (_keyFrames.size() > 0){
    //_currentKeyFrameIter->printKeyFrame();
    if (_currentFrameId < 0){
        printf("No current frame yet. \n");
    }
    else {
      printf("Current frame: \n");
      _keyFrames[_currentFrameId].printKeyFrame();
    }
  }
  printf("\n");
}

  KeyFrame& Animation::getKeyFrame(unsigned i){
    if (i < 0 || i >= numberOfKeyFrames() ){
     throw 0;
    }
    return _keyFrames[i];
  }
