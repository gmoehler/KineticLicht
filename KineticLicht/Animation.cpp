#include "Animation.h"

int freeRam1 ()
{
#ifndef WITHIN_UNITTEST
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#else
  return 0;
#endif
}

bool keyFrameCompare (KeyFrame i, KeyFrame j) {
  return (i.getTimeMs()<j.getTimeMs());
}

Animation::Animation() :
   _numberOfKeyFrames(0),
   _initialValuesRetrieved(false),
   _isSorted(false),
   _withMotor(false),
   _animationFinished(false),
   _finishTime(0L) {}

void Animation::init(std::vector<KeyFrame> kfs) {
  _clear();
  addKeyFrames(kfs);
}

void Animation::init(unsigned **v, int length) {
  _clear();
  for (int i=0; i< length; i++){
    KeyFrame kf(v[i]);
    addKeyFrame(kf);
  }
  _doSort();
}

#ifdef WITH_PROGMEM
void Animation::init(_FLASH_TABLE<unsigned> *ftable) {
  _clear();
  int numRows = ftable->rows();
  for (int i=0; i< numRows; i++){
    _FLASH_ARRAY<unsigned> v = (*ftable)[i];
  //  FPRINTF2(ani_msg20, "read %d array   %d bytes", i, freeRam1());
    KeyFrame kf(v);
  //  FPRINTF2(ani_msg21, "read %d keyframe %d bytes", i, freeRam1());
    addKeyFrame(kf);
  //  FPRINTF2(ani_msg22, "read %d add      %d bytes\n", i, freeRam1());
    if (freeRam1() < 200){
        FPRINTF1(kin_msg10, "ERROR! Memory exhausted: %d Bytes left\n", freeRam1());
    }
  }
  _doSort();
  FPRINTF1(ani_msg22, "sorted:      %d bytes\n", freeRam1());

}
#endif

void Animation::_clear() {
  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {
        std::vector<KeyFrame>& kfs = it-> second;
        kfs.clear();
  }

  _numberOfKeyFrames = 0;
  _initialValuesRetrieved = false,
  _isSorted = false,
  _withMotor = false,
  _animationFinished = false,
  _finishTime = 0L;
}

int Animation::numberOfKeyFrames(){
  int num = 0;
  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {
    std::vector<KeyFrame> kfs = it-> second;
    num += kfs.size();
  }

  //return num;

  return _numberOfKeyFrames;
}

 bool Animation::containsMotorFrames(){
 	return _withMotor;
 }

bool Animation::isAnimationFinished(long elapsedTime) {
  return _animationFinished;
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
      std::vector<KeyFrame>& kfs = it-> second;
      if (kfs.size() > 0){
        KeyFrame& currentKeyFrame = kfs[0];
        nextKeyFrames.push_back(currentKeyFrame);
      }
    }
    _initialValuesRetrieved = true;
  }

  // get next key frames
  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {

    uint8_t id = it-> first;
    std::vector<KeyFrame>& kfs = it-> second;
    uint8_t currentFrameId = _currentFrameIdMap[id];

    KeyFrame& currentKeyFrame = kfs[currentFrameId];

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
        FPRINTF1(ani_msg8, "*** %d: Finished Sub-animation\n", id);
      }

      // look at next frame already
      currentTargetTime = currentKeyFrame.getTimeMs();
    }
  }

  if (elapsedTime > _finishTime){
    _animationFinished = true;
    FPRINTF0(ani_msg8, "*** Finished Animation\n");
  }

  return nextKeyFrames;
}

void Animation::_doSort(){
  FPRINTF0(ani_msg6, "Sorting animation...");

  for(auto it = _keyFrameMap.begin(); it != _keyFrameMap.end(); it++) {
    std::vector<KeyFrame>& kfs = it-> second;
    std::sort (kfs.begin(), kfs.end(), keyFrameCompare);
  }

  _isSorted = true;
  resetCurrentKeyFrame();

  FPRINTF0(ani_msg7, "done.\n");
}

void Animation::addKeyFrame(KeyFrame& kf) {
  //  FPRINTF1(ani_msg25, "addKeyFrame1   %d bytes ", freeRam1());
    uint8_t id = kf.getId();
  //  FPRINTF1(ani_msg28, "addKeyFrame1a   %d bytes ", freeRam1());
    KeyFrame kkf(0,0,0);
  //  FPRINTF1(ani_msg29, "addKeyFrame1b   %d bytes ", freeRam1());
    _keyFrameMap[id].push_back(kf);
//    FPRINTF1(ani_msg26, "addKeyFrame2   %d bytes ", freeRam1());
//    printf("add to Map %d size %d %d\n", id, _keyFrameMap.size(), _keyFrameMap[id].size());
    if (kf.getTimeMs() > _finishTime){
      _finishTime = kf.getTimeMs();
    }
    if (kf.getType() == MOTORFRAME) {
      _withMotor = true;
    }

    _numberOfKeyFrames++;

    // trigger resorting
    _isSorted = false;
    //FPRINTF1(ani_msg27, "addKeyFrame3   %d bytes\n", freeRam1());
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
      KeyFrame& ckf = kfs[currentId];
      ckf.printKeyFrame();
    }
  }

  FPRINTF0(ani_msg5, "\n");
}
