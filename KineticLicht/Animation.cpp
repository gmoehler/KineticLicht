#include "Animation.h"

bool keyFrameCompare (KeyFrame i, KeyFrame j) {
  return (i.getTimeMs()<j.getTimeMs());
}

Animation::Animation() :
   _currentFrameId(-1), _isSorted(true),
   _withMotor(false) {}

Animation::Animation(std::vector<KeyFrame> kfs): Animation() {
  addKeyFrames(kfs);
}

Animation::Animation(unsigned **v, int length): Animation() {
  for (int i=0; i< length; i++){
    KeyFrame kf(v[i]);
    addKeyFrame(kf);
  }
}

#ifndef WITHIN_UNITTEST
Animation::Animation(_FLASH_TABLE* ftable){
  for (int i=0; i< table->rows(); i++){
    _FLASH_ARRAY v = (table*)[i];
    KeyFrame kf(v[i]);
    addKeyFrame(kf);
  }
}
#endif

KeyFrame& Animation::getKeyFrame(int id){
	return _keyFrames[id];
}

int Animation::numberOfKeyFrames(){
  return _keyFrames.size();
}

 bool Animation::containsMotorFrames(){
 	return _withMotor;
 }

bool Animation::isAnimationFinished() {
  return numberOfKeyFrames() == 0 || _currentFrameId >= numberOfKeyFrames() -1;
}

bool Animation::needsTargetFrameUpdate(long elapsedTime) {

  // nothing read so far
  if (_currentFrameId < 0){
    return true;
  }

  double currentTargetTime = getKeyFrame(_currentFrameId).getTimeMs();//_currentKeyFrameIter->getTimeMs();
  //printf("**** currentTargetTime: %f, elapsedTime: %ld\n", currentTargetTime, elapsedTime);

  return (currentTargetTime < elapsedTime);
}

bool Animation::nextFrameWithSameTime() {
  // nothing read so far
  if (_currentFrameId < 0){
    return true;
  }
  if (isAnimationFinished()){
    return false;
  }

  double currentTargetTime = getKeyFrame(_currentFrameId).getTimeMs();//_currentKeyFrameIter->getTimeMs();
  double nextTargetTime = getKeyFrame(_currentFrameId+1).getTimeMs();//_currentKeyFrameIter->getTimeMs();

return currentTargetTime == nextTargetTime;
}

std::vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  // need resorting
  if (!_isSorted){
    std::sort (_keyFrames.begin(), _keyFrames.end(), keyFrameCompare);
    if (numberOfKeyFrames() > 0){
      _currentFrameId = -1;
      _isSorted = true;
    }
  }

  std::vector<KeyFrame> nextKeyFrames;

  while (!isAnimationFinished() &&
    (needsTargetFrameUpdate(elapsedTime) || nextFrameWithSameTime())){
    // for first frame the iterator is already pointing to the correct frame
    _currentFrameId++;
    nextKeyFrames.push_back(getKeyFrame(_currentFrameId));
  }

  return nextKeyFrames;
}

void Animation::addKeyFrame(KeyFrame kf) {
    _keyFrames.push_back(kf);
    if (kf.getType() == MOTORFRAME) {
      _withMotor = true;
    }

    // trigger resorting
    _isSorted = false;
}

void Animation::addKeyFrames(std::vector<KeyFrame> kfs) {

  // need loop to account for _withMotor instead of one-line insert
  for (std::vector<KeyFrame>::iterator it = kfs.begin() ; it != kfs.end(); ++it) {
    addKeyFrame(*it);
  }
}

void Animation::resetCurrentKeyFrame(){
  if (numberOfKeyFrames() > 0){
    //_currentKeyFrameIter = _keyFrames.begin();
    _currentFrameId = 0;
  }
}

void Animation::printAnimation(){
  printf("Animation contains %d frames ", numberOfKeyFrames());
  if (_withMotor){
    printf("with motor frames.\n" );
  }
  else {
    printf("with no motor frames.\n" );
  }

  if (numberOfKeyFrames() > 0){
    if (_currentFrameId < 0){
        printf("No current frame. \n");
    }
    else {
      printf("Current frame: \n");
      getKeyFrame(_currentFrameId).printKeyFrame();
    }
  }

  printf("\n");
}
