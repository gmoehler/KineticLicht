#ifndef ANIMATIONSTORE_H
#define ANIMATIONSTORE_H

#include "KeyFrame.h"
#include "Animation.h"
#include "RgbLED.h"

class AnimationStore{

public:
  AnimationStore();

  Animation& getAnimation(int id);

private:
  Animation animation1;

};

#endif
