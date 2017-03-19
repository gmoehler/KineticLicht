#ifndef ANIMATION_H
#define ANIMATION_H

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#else
  #include <stdio.h>
  #include "mock_Arduino.h"
#endif

#ifdef WITH_PROGMEM
  #include <Flash.h>
#endif

#include <map>
#include <vector>
#include <algorithm>    // std::sort

#include "KineticLicht.h"
#include "KeyFrame.h"

enum LedPosition { TOP, BOTTOM };

class Animation
{
public:

  Animation();
  Animation(std::vector<KeyFrame> new_kfs);
  Animation(unsigned **v, int length);
#ifdef WITH_PROGMEM
  Animation(_FLASH_TABLE<unsigned>* ftable);
#endif

  bool isAnimationFinished();
  // bool needsTargetFrameUpdate(long elapsedTime);
  //bool nextFrameWithSameTime();
  std::vector<KeyFrame> getNextTargetKeyFrames(long elapsedTime);

  // add new key frames - sort is done on first getNextTargetKeyFrames()
  void addKeyFrame(KeyFrame kf);
  void addKeyFrames(std::vector<KeyFrame> new_kfs);
  //KeyFrame& getKeyFrame(int id);
  void resetCurrentKeyFrame();
  int numberOfKeyFrames();

  bool containsMotorFrames();

  void printAnimation();

private:
  //std::vector<KeyFrame> _keyFrames;
  std::map<uint8_t, std::vector<KeyFrame>> _keyFrameMap;
  //int _currentFrameId;
  std::map<uint8_t, uint8_t> _currentFrameIdMap;
  //unsigned _numberOfKeyFrames;
  bool _isSorted;
  bool _withMotor;
  void _doSort();
};

#endif
