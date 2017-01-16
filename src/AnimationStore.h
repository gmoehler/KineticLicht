#ifndef ANIMATIONSTORE_H
#define ANIMATIONSTORE_H

#include <ArduinoSTL.h>

#include "KeyFrame.h"
#include "Animation.h"

using namespace std;

class AnimationStore{

public:
  AnimationStore();

  Animation& getAnimation(int id);

  int getNumAnimations();

private:
  vector<Animation> _animation;

};

#endif
