#ifndef ANIMATIONOPS_H
#define ANIMATIONOPS_H

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
#include "AnimationList.h"
#include "StepperWorker.h"
#include "LedWorker.h"

enum AnimationState { ANIMATION_INIT,           // 0
                      ANIMATION_CALIBRATING,    // 1
                      ANIMATION_ACTIVE,         // 2
                      ANIMATION_FINISHED,       // 3
                      NUM_ANIMATION_STATES};    // only used for enum size

enum AnimationStrategy { SINGLE_ANIMATION, LOOP_ANIMATION };

#define NO_CURRENT_ANIMATION -1

class AnimationOps : public FiniteStateMachine<AnimationOps> {

public:
  AnimationOps(Adafruit_TLC5947& tlc, bool loadAnimations=true);

  void addStepperWorker(StepperWorker* sw);
  void addLedWorker(LedWorker* lw);

  // choose an animation to go in production
  void selectAnimation(int8_t id);
  Animation& _getCurrentAnimation();
  int8_t getNumAnimations();

  // to be called in setup()
  void init(AnimationStrategy strategy, int8_t startWithAnimationId, bool repeat);
  void loop(); // to be called in loop()

private:
  AnimationList _animations;
  std::map<int,StepperWorker*> _stepperWorkerMap;
  std::map<int,LedWorker*> _ledWorkerMap;

  Animation _currentAnimation;
  int8_t _currentAnimationId; // -1 if no current animation
  long _elapsedTime;
  long _startTime;

  AnimationStrategy _strategy;
  int8_t _strategy_startWithAnimationId;
  bool _strategy_repeat;

  bool _debug;

  Adafruit_TLC5947 _tlc;

  bool _init_to_calibrating();
  bool _init_to_active();
  bool _calibrating_to_active();
  bool _finished_to_init();

  void _entry_calibrating();
  void _action_calibrating();

  void _entry_active();
  void _action_active();

  void _entry_finished();
  void _action_finished();

};

#endif
