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
    _numKeyFrames = new int[_numAnimations];
    //_allAnimations = new animation_as_uint_t*[_numAnimations];
    if (loadAnimations){
      load();
    }
  }

  ~AnimationList() {
    // recursively deleting multidim array
    for (int i=0; i <_numAnimations; i++){
      /*for (int j=0; j < _numKeyFrames[i]; j++){
      animation_as_uint_t *kfs = _allAnimations[i];
      delete[] &kfs[0][0];
    }*/
    delete[] _allAnimations[i];
  }
  delete[] _numKeyFrames;
}

void load();

int getNumAnimations(){
  return _numAnimations;
}

animation_as_uint_t* getAnimationAsUint(int id){
  return _allAnimations[id];
}

// number of key frames for each animation
int getNumKeyFrames(int id){
  return _numKeyFrames[id];
}


private:
  int _numAnimations;
  //animation_as_uint_t** _allAnimations;
  animation_as_uint_t* _allAnimations[4];
  int* _numKeyFrames;

  // add an animation array to _allAnimations
  void _addAsAnimationUint(unsigned v[][7], int rows, int idx) {
    if (idx >= getNumAnimations() || idx < 0){
      printf("Cannot store animation uint at index %d, max index is %d.\n", idx, getNumAnimations());
    }

    // create a copy on the heap
    auto v_heap = new unsigned[rows][7]();
    std::copy(&v[0][0], &v[0][0]+rows*7,&v_heap[0][0]);
    _allAnimations[idx] = v_heap;
    _numKeyFrames[idx] = rows;
  }
};

#endif
