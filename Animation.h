#ifndef ANIMATION_H
#define ANIMATION_H

#include <ArduinoSTL.h>
#include <map>

#include "KeyFrame.h"

using namespace std;

enum LedPosition { TOP, BOTTOM };
enum Activators {STEPPER1, STEPPER2, STEPPER3, STEPPER4, LED1TOP, LED1BOT, LED2TOP, LED2BOT, LED3TOP, LED3BOT, LED4TOP, LED4BOT};


class Animation
{
public:

  Animation();

  bool isAnimationFinished();
  bool hasNextTargetKeyFrame(long elapsedTime);
  vector<KeyFrame> getNextTargetKeyFrames(long elapsedTime);

  // add new key frames - assuming that they are time sorted for now
  void addKeyFrames(vector<KeyFrame> new_kfs);

private:
  vector<KeyFrame> keyFrames;
  vector<KeyFrame>::iterator _currentKeyFrameIter;


};


#endif

