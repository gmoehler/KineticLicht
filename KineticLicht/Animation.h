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

typedef unsigned int animation_as_uint_t [8];

enum LedPosition { TOP, BOTTOM };

class Animation
{
public:

  Animation();
  Animation(vector<KeyFrame> new_kfs);
  Animation(animation_as_uint_t *v, unsigned length);

  bool isAnimationFinished();
  bool needsTargetFrameUpdate(long elapsedTime);
  vector<KeyFrame> getNextTargetKeyFrames(long elapsedTime);

  // add new key frames - sort is done on first getNextTargetKeyFrames()
  void addKeyFrame(KeyFrame kf);
  void addKeyFrames(vector<KeyFrame> new_kfs);
  KeyFrame& getKeyFrame(unsigned id);
  void resetCurrentKeyFrame();
  unsigned numberOfKeyFrames();

  bool containsMotorFrames();

  void printAnimation();

private:
  vector<KeyFrame> _keyFrames;
  int _currentFrameId;
  bool _isSorted;
  bool _withMotor;
  KeyFrame _emptyKeyFrame; // return this one in case of errors
};

#endif
