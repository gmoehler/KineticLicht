#ifndef ANIMATION_H
#define ANIMATION_H
#ifndef TEST
#include <ArduinoSTL.h>
#endif
#include <map>

#include "KeyFrame.h"
#include <algorithm>    // std::sort

using namespace std;

enum LedPosition { TOP, BOTTOM };
enum Activators {STEPPER1, STEPPER2, STEPPER3, STEPPER4,
                 LED1TOP, LED1BOT, LED2TOP, LED2BOT, LED3TOP, LED3BOT, LED4TOP, LED4BOT};


class Animation
{
public:

  Animation();

  bool isAnimationFinished();
  bool hasNextTargetKeyFrame(long elapsedTime);
  vector<KeyFrame> getNextTargetKeyFrames(long elapsedTime);

  // add new key frames - sort is done on first getNextTargetKeyFrames()
  void addKeyFrames(vector<KeyFrame> new_kfs);

  void printAnimation();

private:
  vector<KeyFrame> _keyFrames;
  vector<KeyFrame>::iterator _currentKeyFrameIter;
  bool _isSorted;
};

#endif
