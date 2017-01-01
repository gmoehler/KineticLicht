#ifndef ANIMATIONSTORE_H
#define ANIMATIONSTORE_H

#include <ArduinoSTL.h>

#include "KeyFrame.h"
#include "Animation.h"
#include "RgbLED.h"

using namespace std;

class AnimationStore{

public:
  AnimationStore();

  Animation& getAnimation(int id);

private:
  Animation animation1;

};

#endif
