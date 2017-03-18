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
  _doSort();
}

#ifdef WITH_PROGMEM
Animation::Animation(_FLASH_TABLE<unsigned> *ftable){
  int numRows = ftable->rows();
  for (int i=0; i< numRows; i++){
    _FLASH_ARRAY<unsigned> v = (*ftable)[i];
    KeyFrame kf(v);
    addKeyFrame(kf);
  }
  _doSort();
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

  double currentTargetTime = getKeyFrame(_currentFrameId).getTimeMs();
  //FPRINTF2(ani_msg6, "**** currentTargetTime: %f, elapsedTime: %ld\n", currentTargetTime, elapsedTime);

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

  double currentTargetTime = getKeyFrame(_currentFrameId).getTimeMs();
  double nextTargetTime = getKeyFrame(_currentFrameId+1).getTimeMs();

return currentTargetTime == nextTargetTime;
}

std::vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  // need resorting
  if (!_isSorted){
    _doSort();
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

void Animation::_doSort(){
  FPRINTF0(ani_msg6, "Sorting animation...");

  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {
    std::vector<KeyFrame> kfs = it-> second;
    std::sort (kfs.begin(), kfs.end(), keyFrameCompare);
  }

  std::sort (_keyFrames.begin(), _keyFrames.end(), keyFrameCompare);
  if (numberOfKeyFrames() > 0){
    _currentFrameId = -1;
    _isSorted = true;
  }
  FPRINTF0(ani_msg7, "done.\n");
}

void Animation::addKeyFrame(KeyFrame kf) {
    uint8_t id = kf.getId();
    _keyFrameMap[id].push_back(kf);
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
    _currentFrameId = 0;
  }
}

void Animation::printAnimation(){
  FPRINTF1(ani_msg0, "Animation contains %d frames ", numberOfKeyFrames());
  if (_withMotor){
    FPRINTF0(ani_msg1,"with motor frames.\n" );
  }
  else {
    FPRINTF0(ani_msg2,"with no motor frames.\n" );
  }

  if (numberOfKeyFrames() > 0){
    if (_currentFrameId < 0){
        FPRINTF0(ani_ms32,"No current frame. \n");
    }
    else {
      FPRINTF0(ani_msg4, "Current frame: \n");
      getKeyFrame(_currentFrameId).printKeyFrame();
    }
  }

  FPRINTF0(ani_msg5, "\n");
}
