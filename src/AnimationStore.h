#ifndef ANIMATIONSTORE_H
#define ANIMATIONSTORE_H

#ifdef WITHIN_UNITTEST
  #include <stdio.h>
#else
  #include <ArduinoSTL.h>
#endif

#include "KeyFrame.h"
#include "Animation.h"

using namespace std;

class AnimationStore{

public:
  AnimationStore();

  int addAnimation(Animation );
  Animation& getAnimation(int id);

  int getNumAnimations();

private:
  vector<Animation> _animation;

};

#endif
