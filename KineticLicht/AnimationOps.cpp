#include "AnimationOps.h"

// full height: 4200 = 2100mm

int AnimationOps::addAnimation(Animation& a){
  _animation.push_back(a);
  return _animation.size() -1;
}

int AnimationOps::getNumAnimations(){
  return _animation.size();
}

Animation& AnimationOps::getAnimation(int id){
  printf("AnimationOps: Selecting animation %d:\n", id);
  _animation.at(id).printAnimation();
  return _animation.at(id);
}

Animation& AnimationOps::_getCurrentAnimation(){
  if (_animation.size() == 0 || (int) _animation.size() < _currentAnimationId){
    printf("AnimationOps: Cannot select current Animation %d:\n", _currentAnimationId);
  }
  return getAnimation(_currentAnimationId);
}

void AnimationOps::init(AnimationStrategy strategy,
  int startWithAnimationId, bool repeat){

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

  if (getNumAnimations() > _strategy_startWithAnimationId){
    _currentAnimationId = _strategy_startWithAnimationId;
  }
  else {
    printf ("#### ERROR! Cannot start with animation id %d > %d.\n",
    _strategy_startWithAnimationId, getNumAnimations());
  }

}

void AnimationOps::loop(){
    FiniteStateMachine::loop();
}

void AnimationOps::addStepperWorker(StepperWorker* sw){
  int id = sw->getId();
  // cannot use operator[] since we do not have an empty constructor of StepperWorker
  _stepperWorkerMap.insert( std::map<int, StepperWorker*>::value_type ( id, sw ));
}

void AnimationOps::addLedWorker(LedWorker* lw){
  int id = lw->getId();
  // cannot use operator[] since we do not have an empty constructor of LedWorker
  _ledWorkerMap.insert( std::map< int, LedWorker* >::value_type ( id, lw ));
}

bool AnimationOps::_init_to_calibrating(){
  printf("_init_to_calibrating %d:\n", _getCurrentAnimation().numberOfKeyFrames());
  return _getCurrentAnimation().containsMotorFrames();
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

void AnimationOps::_entry_calibrating(){
  printf("### Proceeding to state ANIMATION_CALIBRATING. ###\n");
  _startTime = millis(); // reset time
  printf("+++ startTime: %ld\n", _startTime);
  for (auto it = _stepperWorkerMap.begin() ;
  it != _stepperWorkerMap.end(); ++it) {
    StepperWorker* sw = it->second;
    printf("+++ Starting calibration for sw %d\n", sw->getId());
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

bool AnimationOps::_init_to_active(){
  if (!_animation[_currentAnimationId].containsMotorFrames()){
    printf("### No motor frames. Proceeding directly to state ANIMATION_ACTIVE. ###\n");
    return true;
  }
  return false;
}

void AnimationOps::_entry_active(){
  _startTime = millis(); // reset time
  printf("+++ startTime: %ld\n", _startTime);
  for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
    StepperWorker* sw = it->second;
    sw->startAnimation();
  }
}

void AnimationOps::_entry_finished(){
  // choose next animation based on strategy
  if (_strategy == SINGLE_ANIMATION) {
    // stop after first animation
    _currentAnimationId = -1;
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
        _currentAnimationId = -1;
      }
    }
  }
}

void AnimationOps::_action_finished(){
  if (_currentAnimationId == -1){
    //TODO: do things to stop animation: go dark, stop steppers, ...
  }
}

bool AnimationOps::_finish_to_calibrating(){
  // continue if we have a valid id
  if (_currentAnimationId < 0){
    printf("No more animations available");
  }
  else {
    printf("### Proceeding with Animation %d ###.\n", _currentAnimationId);
  }

  if (_currentAnimationId >= 0){
    _getCurrentAnimation().resetCurrentKeyFrame();
    return true;
  }
  return false;
}

void AnimationOps::_action_active(){

  _elapsedTime = millis() - _startTime;
  printf("+++ elapsed Time: %ld\n", _elapsedTime);
  _getCurrentAnimation().printAnimation();

  if (_getCurrentAnimation().needsTargetFrameUpdate(_elapsedTime)) {
    vector<KeyFrame> kfs = _getCurrentAnimation().getNextTargetKeyFrames(_elapsedTime);
    // no more frames - animation is at an end
    if (kfs.size() == 0){
      printf("*********************** Need more frames, but there are none\n");
      triggerTransition(getState(), ANIMATION_FINISHED);
      return;
    }

    for (vector<KeyFrame>::iterator kf_it = kfs.begin(); kf_it != kfs.end(); kf_it++) {
      KeyFrame kf = *kf_it;

      bool keyFrameHandled = false;
      // check whether this is an update for a stepper worker
      auto sit =_stepperWorkerMap.find(kf_it->getId());
      if(sit != _stepperWorkerMap.end()) {
        StepperWorker* sw = sit->second;
        sw->updateTargetKeyFrame(_elapsedTime, kf);
        keyFrameHandled = true;
   }

      // check whether this is an update for a stepper worker
      auto lit =_ledWorkerMap.find(kf_it->getId());
      if(lit != _ledWorkerMap.end()) {
        LedWorker* lw = lit->second;
        lw->updateTargetKeyFrame(_elapsedTime, kf);
        keyFrameHandled = true;
      }

      if (!keyFrameHandled){
        printf("### WARNING. KeyFrame id did not match any worker: %d ###.\n", kf_it->getId());
	    }
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
      _tlc.setLED(lw->getId(), rgbColor.red(), rgbColor.green(), rgbColor.blue());
    }
    _tlc.write();
  }

}

AnimationOps::AnimationOps(Adafruit_TLC5947& tlc)
: FiniteStateMachine (NUM_ANIMATION_STATES, ANIMATION_INIT, *this),
_currentAnimationId(-1), _elapsedTime(0), _startTime(-1),
_strategy(SINGLE_ANIMATION), _strategy_startWithAnimationId(-1), _strategy_repeat(false),
_tlc(tlc)
{
  setDebugString(string("AnimationOps"));
  _tlc = tlc;

  addTransition(ANIMATION_INIT, ANIMATION_CALIBRATING, &AnimationOps::_init_to_calibrating);
  addTransition(ANIMATION_INIT, ANIMATION_ACTIVE, &AnimationOps::_init_to_active);
  addTransition(ANIMATION_CALIBRATING, ANIMATION_ACTIVE, &AnimationOps::_calibrating_to_active);
  addTransition(ANIMATION_FINISHED, ANIMATION_CALIBRATING, &AnimationOps::_finish_to_calibrating);

  addStateEntryAction(ANIMATION_CALIBRATING,&AnimationOps::_entry_calibrating);
  addStateAction(ANIMATION_CALIBRATING, &AnimationOps::_action_calibrating);
  addStateEntryAction(ANIMATION_ACTIVE, &AnimationOps::_entry_active);
  addStateAction(ANIMATION_ACTIVE, &AnimationOps::_action_active);
  addStateEntryAction(ANIMATION_FINISHED, &AnimationOps::_entry_finished);
  addStateAction(ANIMATION_FINISHED, &AnimationOps::_action_finished);

  }