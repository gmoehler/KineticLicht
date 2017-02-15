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

enum AnimationState { ANIMATION_INIT,
                      ANIMATION_CALIBRATING,
                      ANIMATION_ACTIVE,
                      ANIMATION_FINISHED,
                      NUM_ANIMATION_STATES};

class AnimationStore : public FiniteStateMachine<AnimationStore> {

public:
  AnimationStore();

  void addStepperWorker(StepperWorker sw);
  void addLedWorker(LedWorker lw);

  int addAnimation(Animation );
  Animation& getAnimation(int id);

  int getNumAnimations();

private:
  vector<Animation> _animation;
  std::map<int,StepperWorker> _stepperWorkerMap;
  std::map<int,LedWorker> _ledWorkerMap;

  int _currentAnimationId;


  bool _init_to_calibrating();
  bool _init_to_active();
  bool _calibrate_to_active();
  bool _active_to_finish();

  void _entry_calibrating();
  void _action_calibrating();

  void _entry_active();
  void _action_active();

};

#endif
