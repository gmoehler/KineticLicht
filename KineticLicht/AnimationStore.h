#ifndef ANIMATIONSTORE_H
#define ANIMATIONSTORE_H

#ifdef WITHIN_UNITTEST
  #include <stdio.h>
#else
  #include <ArduinoSTL.h>
#endif

#include "FiniteStates.h"
#include "KeyFrame.h"
#include "Animation.h"
#include "StepperWorker.h"
#include "LedWorker.h"


using namespace std;

enum AnimationState {ANIMATION_ACTIVE, ANIMATION_INIT, ANIMATION_CALIBRATING, ANIMATION_FINISHED,
                                         NUM_ANIMATION_STATES};

class AnimationStore : public FiniteStateMachine<AnimationStore> {

public:
  AnimationStore();

  void addStepperWorker(StepperWorker);
  void addLedWorker(LedWorker);
  
  int addAnimation(Animation );
  Animation& getAnimation(int id);

  int getNumAnimations();

private:
  vector<Animation> _animation;
  vector<StepperWorker> _stepperWorker;
  vector<LedWorker> _ledWorker;
  
  
  bool _init_to_calibrate();
  bool _init_to_active();
  bool _calibrate_to_active();
  bool _active_to_finish();
  
  void _action_active();
  void _action_calibrate();
  

};

#endif
