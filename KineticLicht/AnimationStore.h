#ifndef ANIMATIONSTORE_H
#define ANIMATIONSTORE_H

#ifdef WITHIN_UNITTEST
  #include <stdio.h>
  #include "../test/mock_Arduino.h"
#else
  #include <ArduinoSTL.h>
  #include "Adafruit_TLC5947.h"
#endif

#include "FiniteStates.h"
#include "KeyFrame.h"
#include "Animation.h"
#include "StepperWorker.h"
#include "LedWorker.h"



using namespace std;

enum AnimationState { ANIMATION_INIT,           // 0
                      ANIMATION_CALIBRATING,    // 1
                      ANIMATION_ACTIVE,         // 2
                      ANIMATION_FINISHED,       // 3
                      NUM_ANIMATION_STATES};    // only used for enum size

enum AnimationStrategy { SINGLE, LOOP };

class AnimationStore : public FiniteStateMachine<AnimationStore> {

public:
  AnimationStore();

  void addStepperWorker(StepperWorker* sw);
  void addLedWorker(LedWorker* lw);

  int addAnimation(Animation& a );
  Animation& getAnimation(int id);

  int getNumAnimations();

  void init(Adafruit_TLC5947& tlc, AnimationStrategy strategy,
    int startWithAnimationId, bool repeat); // to be called in setup()
  void loop(); // to be called in loop()

private:
  vector<Animation> _animation;
  std::map<int,StepperWorker*> _stepperWorkerMap;
  std::map<int,LedWorker*> _ledWorkerMap;

  int _currentAnimationId;
  long _elapsedTime;
  long _startTime;

  AnimationStrategy _strategy;
  int _strategy_startWithAnimationId;
  bool _strategy_repeat;

  Adafruit_TLC5947 _tlc;

  Animation& _getCurrentAnimation();

  bool _init_to_calibrating();
  bool _init_to_active();
  bool _calibrating_to_active();
  bool _finish_to_calibrating();

  void _entry_calibrating();
  void _action_calibrating();

  void _entry_active();
  void _action_active();

  void _entry_finished();
  void _action_finished();

};

#endif
