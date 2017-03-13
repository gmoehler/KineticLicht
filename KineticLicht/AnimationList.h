// AnimationList.h

#ifndef ANIMATIONLIST_H
#define ANIMATIONLIST_H

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#else
  #include "../test/mock_Arduino.h"
#endif

#include <algorithm>
#include "Animation.h"

#define NUM_COLS 7

class AnimationList
{

public:
  AnimationList(bool loadAnimations=true) : _numAnimations(0){
    if (loadAnimations){
      load();
    }
  }

  ~AnimationList() {
    // recursively deleting multidim array
    for (uint8_t i=0; i <_numAnimations; i++){
      for (uint8_t j=0; j < _numKeyFrames[i]; j++){
        delete[] _allAnimations[i][j];
    }
    delete[] _allAnimations[i];
  }
  delete [] _allAnimations;

  delete[] _numKeyFrames;
}

void load();

// needs to be called before we can use _addAsAnimationUint()
void initNumberOfAnimations(uint8_t numAnimations){
  _numAnimations = numAnimations;
  _numKeyFrames = new int[_numAnimations];
  _allAnimations = new unsigned **[_numAnimations];
}

uint8_t getNumAnimations(){
  return _numAnimations;
}

unsigned **getAnimationAsUint(uint8_t id){
  return _allAnimations[id];
}

// number of key frames for each animation
int getNumKeyFrames(uint8_t id){
  return _numKeyFrames[id];
}


private:
  uint8_t _numAnimations;
  unsigned ***_allAnimations;
  //animation_as_uint_t* _allAnimations[4];
  int* _numKeyFrames;

  // add an animation array to _allAnimations
  // need to call initNumberOfAnimations(numAnimations) before this method
  void _addAsAnimationUint(unsigned v[][NUM_COLS], int rows, uint8_t idx) {
    if (idx >= getNumAnimations() || idx < 0){
      printf("Cannot store animation uint at index %d, max index is %d.\n", idx, getNumAnimations());
    }

    // create a copy on the heap
    // first allocate rows
    _numKeyFrames[idx] = rows;
    _allAnimations[idx] = new unsigned*[rows];
    for(uint8_t j= 0; j < rows; ++j) {
      // then allocate columns
      _allAnimations[idx][j] = new unsigned[NUM_COLS];
      // then copy over values from initialized array
      for(uint8_t k=0; k< NUM_COLS; ++k) {
        _allAnimations[idx][j][k] = v[j][k];
      }
    }
  }

};

#endif
