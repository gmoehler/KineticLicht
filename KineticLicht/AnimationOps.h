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

using namespace std;

//enum Activators {STEPPER1, STEPPER2, STEPPER3, STEPPER4,
//                 LED1TOP, LED1BOT, LED2TOP, LED2BOT, LED3TOP, LED3BOT, LED4TOP, LED4BOT};

enum AnimationState { ANIMATION_INIT,           // 0
                      ANIMATION_CALIBRATING,    // 1
                      ANIMATION_ACTIVE,         // 2
                      ANIMATION_FINISHED,       // 3
                      NUM_ANIMATION_STATES};    // only used for enum size

enum AnimationStrategy { SINGLE_ANIMATION, LOOP_ANIMATION };

class AnimationOps : public FiniteStateMachine<AnimationOps> {

public:
  AnimationOps(Adafruit_TLC5947& tlc, bool loadAnimations=true);

  void addStepperWorker(StepperWorker* sw);
  void addLedWorker(LedWorker* lw);

  int addAnimation(Animation& a );
  
  void selectAnimation(int id);
  int getNumAnimations();

  // to be called in setup()
  void init(AnimationStrategy strategy, int startWithAnimationId, bool repeat);
  void loop(); // to be called in loop()
Animation& _getCurrentAnimation();

private:
  AnimationList _animations;
  std::map<int,StepperWorker*> _stepperWorkerMap;
  std::map<int,LedWorker*> _ledWorkerMap;

  Animation _currentAnimation;
  int _currentAnimationId;
  long _elapsedTime;
  long _startTime;

  AnimationStrategy _strategy;
  int _strategy_startWithAnimationId;
  bool _strategy_repeat;

  Adafruit_TLC5947 _tlc;

  
  Animation& getAnimation(int id);

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
