#ifndef ANIMATION_H
#define ANIMATION_H

#ifdef WITHIN_UNITTEST
  #include <stdio.h>
#else
  #include <ArduinoSTL.h>
  #include <Flash.h>
#endif

#include <vector>
#include <algorithm>    // std::sort

#include "KeyFrame.h"

enum LedPosition { TOP, BOTTOM };

class Animation
{
public:

  Animation();
  Animation(std::vector<KeyFrame> new_kfs);
  Animation(unsigned **v, int length);
#ifndef WITHIN_UNITTEST
  Animation(_FLASH_TABLE* ftable);
#endif

  bool isAnimationFinished();
  bool needsTargetFrameUpdate(long elapsedTime);
  bool nextFrameWithSameTime();
  std::vector<KeyFrame> getNextTargetKeyFrames(long elapsedTime);

  // add new key frames - sort is done on first getNextTargetKeyFrames()
  void addKeyFrame(KeyFrame kf);
  void addKeyFrames(std::vector<KeyFrame> new_kfs);
  KeyFrame& getKeyFrame(int id);
  void resetCurrentKeyFrame();
  int numberOfKeyFrames();

  bool containsMotorFrames();

  void printAnimation();

private:
  std::vector<KeyFrame> _keyFrames;
  int _currentFrameId;
  bool _isSorted;
  bool _withMotor;
  KeyFrame _emptyKeyFrame; // return this one in case of errors
};

#endif
