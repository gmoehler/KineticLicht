#include "Animation.h"

bool keyFrameCompare (KeyFrame i,KeyFrame j) {
  return (i.getTimeMs()<j.getTimeMs());
}

Animation::Animation() :
   _currentFrameId(-1), _isSorted(true),
   _withMotor(false) {}

Animation::Animation(vector<KeyFrame> kfs): Animation() {
  addKeyFrames(kfs);
}

Animation::Animation(animation_as_uint_t *v, unsigned length): Animation() {
  for (unsigned i=0; i< length; i++){
    KeyFrame kf(v[i]);
    addKeyFrame(kf);
  }
}

KeyFrame& Animation::getKeyFrame(unsigned id){
	return _keyFrames[id];
}

unsigned Animation::numberOfKeyFrames(){
  return _keyFrames.size();
}

 bool Animation::containsMotorFrames(){
 	return _withMotor;
 }

bool Animation::isAnimationFinished() {
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

  double currentTargetTime = getKeyFrame(_currentFrameId).getTimeMs();//_currentKeyFrameIter->getTimeMs();
  printf("**** currentTargetTime: %f, elapsedTime: %ld\n", currentTargetTime, elapsedTime);
  return (currentTargetTime < elapsedTime);
}

vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  // need resorting
  if (!_isSorted){
    std::sort (_keyFrames.begin(), _keyFrames.end(), keyFrameCompare);
    if (numberOfKeyFrames() > 0){
      _currentFrameId = -1;
      _isSorted = true;
    }
  }

  vector<KeyFrame> nextKeyFrames;

  while (!isAnimationFinished() && needsTargetFrameUpdate(elapsedTime)){
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

void Animation::addKeyFrames(vector<KeyFrame> kfs) {

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
        printf("No current frame yet. \n");
    }
    else {
      printf("Current frame: \n");
      getKeyFrame(_currentFrameId).printKeyFrame();
    }
  }

  printf("\n");
}
