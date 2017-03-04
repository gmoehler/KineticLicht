// AnimationList.h

#ifndef ANIMATIONLIST_H
#define ANIMATIONLIST_H

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#endif

#include <algorithm>
#include "Animation.h"

class AnimationList
{

public:
  AnimationList(bool loadAnimations=true) : _numAnimations(4){
    _numKeyFrames = new int[4];
    //_allAnimations = new animation_as_uint_t[4];
    if (loadAnimations){
      load();
    }
  }

 ~AnimationList() {
 	for (int i=0;i<4;i++){
 	  delete[] _allAnimations[i];
   }
 }

  void load();

  int getNumAnimations(){
    return _numAnimations;
  }

  animation_as_uint_t* getAnimationAsUint(int id){
    return _allAnimations[id];
  }

  int getNumKeyFrames(int id){
  	return _numKeyFrames[id];
  	}


private:
  int _numAnimations;
  animation_as_uint_t* _allAnimations[4];
  int* _numKeyFrames;

  // add an animation array to _allAnimations
  void _addAsAnimationUint(unsigned v[][8], int rows, int idx) {
    if (idx >= getNumAnimations() || idx < 0){
      printf("Cannot store animation uint at index %d, max idex is %d.\n", idx, getNumAnimations());
    }

    auto v_heap = new unsigned[rows][8]();
    std::copy(&v[0][0], &v[0][0]+rows*8,&v_heap[0][0]);
    _allAnimations[idx] = v_heap;
    _numKeyFrames[idx] = rows;
  }
};

#endif
