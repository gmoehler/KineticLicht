// AnimationList.h
// Class holding a list of animations

#ifndef ANIMATIONLIST_H
#define ANIMATIONLIST_H

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#else
  #include "../test/mock_Arduino.h"
#endif

#ifdef WITH_PROGMEM
  #include <Flash.h>
#endif

#include <algorithm>

#include "KineticLicht.h"
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
#ifdef WITH_PROGMEM
      delete _allAnimationTables[i];
#endif
      for (uint8_t j=0; j < _numKeyFrames[i]; j++){
        delete[] _allAnimations[i][j];
    }
    delete[] _allAnimations[i];
  }
  delete [] _allAnimations;
#ifdef WITH_PROGMEM
      delete[] _allAnimationTables;
#endif

  delete[] _numKeyFrames;
}

void load();

// needs to be called before we can use _addAsAnimationUint()
void initNumberOfAnimations(uint8_t numAnimations){
  _numAnimations = numAnimations;
  _numKeyFrames = new int[_numAnimations];
  _allAnimations = new unsigned **[_numAnimations];
#ifdef WITH_PROGMEM
  _allAnimationTables = new _FLASH_TABLE<unsigned>*[_numAnimations];
#endif
}

uint8_t getNumAnimations(){
  return _numAnimations;
}

unsigned **getAnimationAsUint(uint8_t id){
  return _allAnimations[id];
}

#ifdef WITH_PROGMEM
_FLASH_TABLE<unsigned> *getAnimationTable(uint8_t id){
  return _allAnimationTables[id];
}
#endif

// number of key frames for each animation
int getNumKeyFrames(uint8_t id){
#ifdef WITH_PROGMEM
_FLASH_TABLE<unsigned>* animationTable = _allAnimationTables[id];
  return animationTable->rows();
#else
  return _numKeyFrames[id];
#endif
}


private:
  uint8_t _numAnimations;
  unsigned ***_allAnimations;
#ifdef WITH_PROGMEM
  _FLASH_TABLE<unsigned>** _allAnimationTables;
#endif

  int* _numKeyFrames;

  // make copy constructor private
  // should not be used because it waists memory
  AnimationList (const AnimationList&);

  // add an animation array to _allAnimations
  // need to call initNumberOfAnimations(numAnimations) before this method
  void _addAsAnimationUint(unsigned v[][NUM_COLS], int rows, uint8_t idx) {
    if (idx >= getNumAnimations() || idx < 0){
      FPRINTF2(al_msg0, "Cannot store animation uint at index %d, max index is %d.\n", idx, getNumAnimations());
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

#ifdef WITH_PROGMEM
  void _addAsAnimationFlashTable(_FLASH_TABLE<unsigned>& table, uint8_t idx) {
    if (idx >= getNumAnimations() || idx < 0){
      FPRINTF2(al_msg1, "Cannot store animation uint at index %d, max index is %d.\n", idx, getNumAnimations());
    }
    _numKeyFrames[idx] = table.rows();
    _allAnimationTables[idx] = new _FLASH_TABLE<unsigned> (table);
  }
#endif

};

#endif
