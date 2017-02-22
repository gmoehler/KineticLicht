// AnimationList.h

#ifndef ANIMATIONLIST_H
#define ANIMATIONLIST_H

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#endif
#include "Animation.h"

enum Activators {STEPPER1, STEPPER2, STEPPER3, STEPPER4,
                 LED1TOP, LED1BOT, LED2TOP, LED2BOT, LED3TOP, LED3BOT, LED4TOP, LED4BOT};

class AnimationList
{

public:
  AnimationList(bool loadAnimations=true){
    if (loadAnimations){
      load();
    }
  }

  void load();

  unsigned getNumAnimations(){
    return _animationList.size();
  }

  unsigned addAnimation(Animation& a){
    _animationList.push_back(a);
    return _animationList.size()-1;
  }

  Animation& getAnimation(unsigned id){
    return _animationList.at(id);
  }

//private:
  std::vector<Animation> _animationList;

};

#endif
