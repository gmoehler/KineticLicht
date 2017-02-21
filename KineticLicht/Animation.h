#ifndef ANIMATION_H
#define ANIMATION_H

#ifdef WITHIN_UNITTEST
  #include <stdio.h>
#else
  #include <ArduinoSTL.h>
#endif

#include <vector>
#include <algorithm>    // std::sort

#include "KeyFrame.h"

using namespace std;

enum LedPosition { TOP, BOTTOM };

class Animation
{
public:

  Animation();
  Animation(vector<KeyFrame> new_kfs);
  Animation(const Animation &a);
  ~Animation();

  bool isAnimationFinished();
  bool needsTargetFrameUpdate(long elapsedTime);
  vector<KeyFrame> getNextTargetKeyFrames(long elapsedTime);

  // add new key frames - sort is done on first getNextTargetKeyFrames()
  void addKeyFrames(vector<KeyFrame> new_kfs);
  void resetCurrentKeyFrame();
  unsigned numberOfKeyFrames();

  void printAnimation();
  KeyFrame& getKeyFrame(unsigned i);

  bool containsMotorFrames();

private:
  vector<KeyFrame> _keyFrames;
  int _currentFrameId;
  bool _isSorted;
  bool _firstTargetFrameRead;
  bool _withMotor;
};

#endif
