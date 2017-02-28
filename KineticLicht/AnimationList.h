// AnimationList.h

#ifndef ANIMATIONLIST_H
#define ANIMATIONLIST_H

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#endif

#include <algorithm>
#include "Animation.h"


enum Activators {STEPPER1, STEPPER2, STEPPER3, STEPPER4,
                 LED1TOP, LED1BOT, LED2TOP, LED2BOT, LED3TOP, LED3BOT, LED4TOP, LED4BOT};

class AnimationList
{

public:
  AnimationList(bool loadAnimations=true) : _numAnimations(4){

    //_allAnimations = new animation_as_uint_t[4];
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

  animation_as_uint_t* getAnimationAsUint(unsigned id){
    return _allAnimations[id];
  }
  
  int getNumKeyFrames(unsigned id){
  	return _numKeyFrames[id];
  	}
  

private:
  std::vector<Animation> _animationList;
  unsigned _numAnimations;
  animation_as_uint_t* _allAnimations[4];
  int _numKeyFrames[4];

  unsigned _getSizeOfAnimationUint(){
    return _numAnimations;
  }

  void _addAsAnimationUint(unsigned v[][8], int rows, unsigned idx) {
    if (idx >= _getSizeOfAnimationUint()){
      printf("Cannot store animation uint at index %d, max idex is %d.\n", idx, _getSizeOfAnimationUint());
    }

    auto v_heap = new unsigned[rows][8]();
    std::copy(&v[0][0], &v[0][0]+rows*8,&v_heap[0][0]);
    _allAnimations[idx] = v_heap;
    _numKeyFrames[idx] = rows;
  }
};

#endif
