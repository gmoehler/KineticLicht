#include "Animation.h"

bool keyFrameCompare (KeyFrame i, KeyFrame j) {
  return (i.getTimeMs()<j.getTimeMs());
}

Animation::Animation() :
   _isSorted(false),
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
/*
KeyFrame& Animation::getKeyFrame(int id){
	return _keyFrames[id];
}
*/
int Animation::numberOfKeyFrames(){
  int num = 0;
  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {
    std::vector<KeyFrame> kfs = it-> second;
    num += kfs.size();
  }

  return num;
}

 bool Animation::containsMotorFrames(){
 	return _withMotor;
 }

bool Animation::isAnimationFinished() {
  // TODO implement this
  //return numberOfKeyFrames() == 0 || _currentFrameId >= numberOfKeyFrames() -1;
  return false;
}

/*
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
*/
std::vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  // need resorting
  if (!_isSorted){
    _doSort();
  }

  std::vector<KeyFrame> nextKeyFrames;

  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {

    uint8_t id = it-> first;
    std::vector<KeyFrame> kfs = it-> second;
    uint8_t currentFrameId = _currentFrameIdMap[id];

    if (currentFrameId == kfs.size()-1){
      // we are at the last key frame
      continue;
    }

    KeyFrame currentKeyFrame = kfs[currentFrameId];

    double currentTargetTime = currentKeyFrame.getTimeMs();

    // read next one if we passed the last target
    while (elapsedTime > currentTargetTime ) {

      ++currentFrameId;
      currentKeyFrame = kfs[currentFrameId];
      nextKeyFrames.push_back(currentKeyFrame);
      _currentFrameIdMap[id] = currentFrameId;

      // look at next frame already
      currentTargetTime = currentKeyFrame.getTimeMs();
    }
  }

/*
  while (!isAnimationFinished() &&
    (needsTargetFrameUpdate(elapsedTime) || nextFrameWithSameTime())){
    // for first frame the iterator is already pointing to the correct frame
    _currentFrameId++;
    nextKeyFrames.push_back(getKeyFrame(_currentFrameId));
  }
*/
  return nextKeyFrames;
}

void Animation::_doSort(){
  FPRINTF0(ani_msg6, "Sorting animation...");

  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {
    std::vector<KeyFrame> kfs = it-> second;
    std::sort (kfs.begin(), kfs.end(), keyFrameCompare);
  }

  _isSorted = true;
  resetCurrentKeyFrame();

/*
  std::sort (_keyFrames.begin(), _keyFrames.end(), keyFrameCompare);
  if (numberOfKeyFrames() > 0){
    _currentFrameId = -1;
    _isSorted = true;
  }*/
  FPRINTF0(ani_msg7, "done.\n");
}

void Animation::addKeyFrame(KeyFrame kf) {
    uint8_t id = kf.getId();
    std::vector<KeyFrame> kfs = _keyFrameMap[id];
    if (kfs.size() == 0){
      // add initial value
      KeyFrame *initial_kf = new KeyFrame(id);
      // TODO: clean up this initial value
      _keyFrameMap[id].push_back(*initial_kf);
    }

    _keyFrameMap[id].push_back(kf);
    //_keyFrames.push_back(kf);
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

  // _currentFrameIdMap might still be empty
  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {
    uint8_t id = it-> first;
    _currentFrameIdMap[id] = 0;
  }


/*
  if (numberOfKeyFrames() > 0){
    _currentFrameId = 0;
  }
  */
}

void Animation::printAnimation(){
  FPRINTF1(ani_msg0, "Animation contains %u frames ", numberOfKeyFrames());
  if (_withMotor){
    FPRINTF0(ani_msg1,"with motor frames.\n" );
  }
  else {
    FPRINTF0(ani_msg2,"with no motor frames.\n" );
  }

  if (numberOfKeyFrames() > 0){

    // need resorting
    if (!_isSorted){
      _doSort();
    }

    FPRINTF0(ani_msg4, "Current key frames: \n");
    for(auto it = _currentFrameIdMap.begin(); it != _currentFrameIdMap.end(); it++) {
      uint8_t id = it-> first;
      uint8_t currentId = it-> second;

      std::vector<KeyFrame> kfs = _keyFrameMap[id];
      KeyFrame ckf = kfs[currentId];
      ckf.printKeyFrame();
    }
  }

  FPRINTF0(ani_msg5, "\n");
}
