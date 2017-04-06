#include "AnimationOps.h"

// full height: 4200 = 2100mm

int freeRam2 ()
{
#ifndef WITHIN_UNITTEST
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#else
  return 0;
#endif
}


AnimationOps::AnimationOps(Adafruit_TLC5947& tlc, bool loadAnimations)
: FiniteStateMachine (NUM_ANIMATION_STATES, ANIMATION_INIT, *this),
_animations(loadAnimations), _currentAnimationId(NO_CURRENT_ANIMATION), _elapsedTime(0), _startTime(-1),
_strategy(SINGLE_ANIMATION), _strategy_startWithAnimationId(-1), _strategy_repeat(false),
_tlc(tlc)
{
  setDebugString(std::string("AnimationOps"));
  _tlc = tlc;

  addTransition(ANIMATION_INIT, ANIMATION_CALIBRATING, &AnimationOps::_init_to_calibrating);
  addTransition(ANIMATION_INIT, ANIMATION_ACTIVE, &AnimationOps::_init_to_active);
  addTransition(ANIMATION_CALIBRATING, ANIMATION_ACTIVE, &AnimationOps::_calibrating_to_active);
  addTransition(ANIMATION_ACTIVE, ANIMATION_FINISHED, &AnimationOps::_active_to_finished);
  addTransition(ANIMATION_FINISHED, ANIMATION_INIT, &AnimationOps::_finished_to_init);

  addStateEntryAction(ANIMATION_CALIBRATING,&AnimationOps::_entry_calibrating);
  addStateAction(ANIMATION_CALIBRATING, &AnimationOps::_action_calibrating);
  addStateEntryAction(ANIMATION_ACTIVE, &AnimationOps::_entry_active);
  addStateAction(ANIMATION_ACTIVE, &AnimationOps::_action_active);
  addStateEntryAction(ANIMATION_FINISHED, &AnimationOps::_entry_finished);
  addStateAction(ANIMATION_FINISHED, &AnimationOps::_action_finished);
}

uint8_t AnimationOps::getNumAnimations(){
  return _animations.getNumAnimations();
}

void AnimationOps::selectAnimation(uint8_t id){
#ifdef WITH_PROGMEM
  _FLASH_TABLE<unsigned> *ftable = _animations.getAnimationTable(id);
  int numKf = _animations.getNumKeyFrames(id);
  FPRINTF1(aop_msg16, "ani selecting. free: %d\n", freeRam2())
  _currentAnimation.init(ftable);
  FPRINTF1(aop_msg14, "ani selected. free: %d\n", freeRam2())
#else
  unsigned **aniUint = _animations.getAnimationAsUint(id);
  int numKf = _animations.getNumKeyFrames(id);
  _currentAnimation.init(aniUint, numKf);
  FPRINTF0(aop_msg13, "Selecting new Animation:\n");
  _currentAnimation.printAnimation();
#endif
}

Animation& AnimationOps::_getCurrentAnimation(){
  return _currentAnimation;
}

void AnimationOps::init(AnimationStrategy strategy,
    uint8_t startWithAnimationId, bool repeat){

    _strategy = strategy;
    _strategy_startWithAnimationId = startWithAnimationId;
    _strategy_repeat = repeat;

    for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
      StepperWorker* sw = it->second;
      sw->init();
    }

    for (auto it = _ledWorkerMap.begin(); it != _ledWorkerMap.end(); ++it) {
      LedWorker* lw = it->second;
      lw->init();
    }

    _tlc.begin(); // start LED PWM decoder

    if (_strategy_startWithAnimationId < getNumAnimations()){
      _currentAnimationId = _strategy_startWithAnimationId;
      selectAnimation(_currentAnimationId);
      FPRINTF1(aop_msg15, "ani selected2. free: %d\n", freeRam2())
    }
    else {
      FPRINTF2 (aops_msg0, "#### ERROR! Cannot start with animation id %d > %d.\n",
      _strategy_startWithAnimationId, getNumAnimations());
    }

  }

  void AnimationOps::loop(){
    FiniteStateMachine::loop();
  }

  void AnimationOps::addStepperWorker(StepperWorker* sw){
    uint8_t id = sw->getId();
    // cannot use operator[] since we do not have an empty constructor of StepperWorker
    _stepperWorkerMap.insert( std::map<int, StepperWorker*>::value_type ( id, sw ));
  }

  void AnimationOps::addLedWorker(LedWorker* lw){
    uint8_t id = lw->getId();
    // cannot use operator[] since we do not have an empty constructor of LedWorker
    _ledWorkerMap.insert( std::map< int, LedWorker* >::value_type ( id, lw ));
  }

  bool AnimationOps::_init_to_calibrating(){
    //FPRINTF1(aops_msg1, "_init_to_calibrating %d:\n", _getCurrentAnimation().numberOfKeyFrames());
    return _getCurrentAnimation().containsMotorFrames();
  }


  void AnimationOps::_entry_calibrating(){
    FPRINTF0(aops_msg2, "### Proceeding to state ANIMATION_CALIBRATING. ###\n");

    //_startTime = millis(); // reset time
    //FPRINTF1(aops_msg3, "+++ startTime: %ld\n", _startTime);
    for (auto it = _stepperWorkerMap.begin() ;
    it != _stepperWorkerMap.end(); ++it) {
      StepperWorker* sw = it->second;
      //FPRINTF1b(aops_msg4, "+++ Starting calibration for sw %d\n", sw->getId());
      sw->startCalibration();
    }
  }

  void AnimationOps::_action_calibrating(){

    _elapsedTime = millis() - _startTime;

    for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
      StepperWorker* sw = it->second;
      sw->loop(_elapsedTime);
    }
  }

  bool AnimationOps::_calibrating_to_active(){
    // return true if all calibrations are finished
    for (auto it = _stepperWorkerMap.begin() ;
    it != _stepperWorkerMap.end(); ++it) {
      StepperWorker* sw = it->second;
      if (sw->getState() != CALIBRATION_FINISHED){
        return false;
      }
    }
    return true;
  }

  bool AnimationOps::_init_to_active(){
    if (!_getCurrentAnimation().containsMotorFrames()){
      FPRINTF0(aops_msg5, "### No motor frames. Proceeding directly to state ANIMATION_ACTIVE. ###\n");
      return true;
    }
    return false;
  }

  void AnimationOps::_entry_active(){
    _startTime = millis(); // reset time
    //FPRINTF1(aops_msg6, "+++ startTime: %ld\n", _startTime);
    for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
      StepperWorker* sw = it->second;
      sw->startAnimation();
    }
  }

  void AnimationOps::_entry_finished(){
    // choose next animation based on strategy
    if (_strategy == SINGLE_ANIMATION) {

      if (!_strategy_repeat){
        // stop after first animation
        _currentAnimationId = NO_CURRENT_ANIMATION;
      }
    }
    else {
      _currentAnimationId++;
      if (_currentAnimationId >= getNumAnimations()) {
        if (_strategy_repeat){
          // restart animation
          _currentAnimationId = _strategy_startWithAnimationId;
        }
        else {
          // stop animation
          _currentAnimationId = NO_CURRENT_ANIMATION;
        }
      }
    }
    // load the new current animation
    if (_currentAnimationId != NO_CURRENT_ANIMATION){
      selectAnimation(_currentAnimationId);
    }
  }

  void AnimationOps::_action_finished(){
    if (_currentAnimationId == NO_CURRENT_ANIMATION){
      //TODO: do things to stop animation: go dark, stop steppers, ...
    }
  }

  bool AnimationOps::_finished_to_init(){

    // continue if we have a valid id
    if (_currentAnimationId == NO_CURRENT_ANIMATION){
      //FPRINTF0(aops_msg7, "No more animations available.\n");
    }
    else {
      FPRINTF1(aops_msg8, "### Proceeding with Animation %d ###.\n", _currentAnimationId);
    }

    if (_currentAnimationId != NO_CURRENT_ANIMATION){
      _getCurrentAnimation().resetCurrentKeyFrame();
      return true;
    }
    return false;
  }

  bool AnimationOps::_active_to_finished(){
    return _getCurrentAnimation().isAnimationFinished(_elapsedTime);
  }

  void AnimationOps::_action_active(){

    _elapsedTime = millis() - _startTime;
#ifdef AOPS_DEBUG
//      FPRINTF1(aops_msg9, "+++ elapsed Time: %ld\n", _elapsedTime);
//      _getCurrentAnimation().printAnimation();
#endif

    std::vector<KeyFrame> kfs = _getCurrentAnimation().getNextTargetKeyFrames(_elapsedTime);
#ifdef AOPS_DEBUG
//      FPRINTF1(aops_msg11, "++++ Number of KeyFrames read: %d\n\n", kfs.size());
#endif
    for (std::vector<KeyFrame>::iterator kf_it = kfs.begin(); kf_it != kfs.end(); kf_it++) {

      bool keyFrameHandled = false;
      // check whether this is an update for a stepper worker
      auto sit =_stepperWorkerMap.find(kf_it->getId());
      if(sit != _stepperWorkerMap.end()) {
        StepperWorker* sw = sit->second;
        sw->updateTargetKeyFrame(_elapsedTime, *kf_it);
        keyFrameHandled = true;
      }

      // check whether this is an update for a stepper worker
      auto lit =_ledWorkerMap.find(kf_it->getId());
      if(lit != _ledWorkerMap.end()) {
        LedWorker* lw = lit->second;
        lw->updateTargetKeyFrame(_elapsedTime, *kf_it);
        keyFrameHandled = true;
      }

      if (!keyFrameHandled){
        FPRINTF1(aops_msg12,"### WARNING. KeyFrame id did not match any worker: %d ###.\n", kf_it->getId());
      }
    }


    for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
      StepperWorker* sw = it->second;
      sw->loop(_elapsedTime);
    }

    bool needLedUpdate = false;

    for (auto it = _ledWorkerMap.begin(); it != _ledWorkerMap.end(); ++it) {
      LedWorker* lw = it->second;
      lw->loop(_elapsedTime);
      if (lw->needsUpdate()){
        needLedUpdate = true;
      }
    }

    if (needLedUpdate) {
      for (auto it = _ledWorkerMap.begin(); it != _ledWorkerMap.end(); ++it) {
        LedWorker* lw = it->second;
        RGB rgbColor = lw->getColorForUpdate();
        _tlc.setLED(lw->getTlc5947_id(), rgbColor.red(), rgbColor.green(), rgbColor.blue());
      }
      _tlc.write();
    }

  } // AnimationOps
