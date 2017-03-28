#include "Animation.h"

int freeRam1 ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

bool keyFrameCompare (KeyFrame i, KeyFrame j) {
  return (i.getTimeMs()<j.getTimeMs());
}

Animation::Animation() :
   _initialValuesRetrieved(false),
   _isSorted(false),
   _withMotor(false),
   _finishedActuators(0),
   _finishTime(0L) {}

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
Animation::Animation(_FLASH_TABLE<unsigned> *ftable): Animation() {
  int numRows = ftable->rows();
  for (int i=0; i< numRows; i++){
    _FLASH_ARRAY<unsigned> v = (*ftable)[i];
    KeyFrame kf(v);
    addKeyFrame(kf);
    if (freeRam1() < 200){
        FPRINTF1(kin_msg10, "ERROR! Memory exhausted: %d Bytes left\n", freeRam1());
    }
  }
  _doSort();
}
#endif

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

bool Animation::isAnimationFinished(long elapsedTime) {
  return _finishedActuators == _keyFrameMap.size() && elapsedTime > _finishTime;
}

std::vector<KeyFrame> Animation::getNextTargetKeyFrames(long elapsedTime) {

  // need resorting
  if (!_isSorted){
    _doSort();
  }

  std::vector<KeyFrame> nextKeyFrames;

  // add initial key frames for all activators
  if (!_initialValuesRetrieved){
    for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {
      std::vector<KeyFrame> kfs = it-> second;
      KeyFrame currentKeyFrame = kfs[0];
      nextKeyFrames.push_back(currentKeyFrame);
      if (kfs.size()==1){
        _finishedActuators++;
        //printf("Finished: %u\n", it-> first);
      }
      _initialValuesRetrieved = true;
    }
  }

  // get next key frames
  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {

    uint8_t id = it-> first;
    std::vector<KeyFrame> kfs = it-> second;
    uint8_t currentFrameId = _currentFrameIdMap[id];

    KeyFrame currentKeyFrame = kfs[currentFrameId];

    if (currentFrameId == kfs.size()-1){
      // we are at the last key frame
      continue;
    }

    double currentTargetTime = currentKeyFrame.getTimeMs();
    bool finished = false;

    // read next one if we passed the last target
    while (elapsedTime > currentTargetTime && !finished) {

      ++currentFrameId;
      currentKeyFrame = kfs[currentFrameId];
      nextKeyFrames.push_back(currentKeyFrame);
      _currentFrameIdMap[id] = currentFrameId;

      if (currentFrameId == kfs.size()-1){
        finished = true;
        _finishedActuators++;
        FPRINTF1(ani_msg8, "*** %d: Finished Animation\n", id);
      }

      // look at next frame already
      currentTargetTime = currentKeyFrame.getTimeMs();
    }
  }

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

  FPRINTF0(ani_msg7, "done.\n");
}

void Animation::addKeyFrame(KeyFrame kf) {
    uint8_t id = kf.getId();
    _keyFrameMap[id].push_back(kf);
//    printf("add to Map %d size %d %d\n", id, _keyFrameMap.size(), _keyFrameMap[id].size());
    if (kf.getTimeMs() > _finishTime){
      _finishTime = kf.getTimeMs();
    }
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

  // we still need to retrieve the first key frames
  _initialValuesRetrieved = false;
  _finishedActuators = 0;

}

void Animation::printAnimation(){
  FPRINTF2(ani_msg0, "Animation finishes at %ld and contains %u frames ", _finishTime, numberOfKeyFrames());
  if (_withMotor){
    FPRINTF0(ani_msg1,"with motor frames.\n" );
  }
  else {
    FPRINTF0(ani_msg2,"with no motor frames.\n" );
  }

  if (numberOfKeyFrames() > 0){

    if (!_initialValuesRetrieved){
      FPRINTF0(ani_msg8, "No key frames in action.\n");
      return;
    }

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
