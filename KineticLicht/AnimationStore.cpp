#include "AnimationStore.h"

// full height: 4200 = 2100mm

int AnimationStore::addAnimation(Animation a){
  _animation.push_back(a);
  return _animation.size() -1;
}

int AnimationStore::getNumAnimations(){
  return _animation.size();
}

Animation& AnimationStore::getAnimation(int id){
  printf("AnimationStore: Selecting animation %d:\n", id);
  _animation.at(id).printAnimation();
  return _animation.at(id);
}

Animation& AnimationStore::_getCurrentAnimation(){
  if (_animation.size() == 0 || (int) _animation.size() < _currentAnimationId){
    printf("AnimationStore: Cannot select current Animation %d:\n", _currentAnimationId);
  }
  return getAnimation(_currentAnimationId);
}

void AnimationStore::setAnimationStrategy(AnimationStrategy strategy, int startWithAnimationId, bool repeat){
  _strategy = strategy;
  _strategy_startWithAnimationId = startWithAnimationId;
  _strategy_repeat = repeat;
}

void AnimationStore::init(Adafruit_TLC5947& tlc){

  _tlc = tlc;

  for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
    StepperWorker sw = it->second;
    sw.init();
  }

  for (auto it = _ledWorkerMap.begin(); it != _ledWorkerMap.end(); ++it) {
    LedWorker lw = it->second;
    lw.init();
  }
}

void AnimationStore::loop(){

}

void AnimationStore::addStepperWorker(StepperWorker sw){
  int id = sw.getId();
  // cannot use operator[] since we do not have an empty constructor or StepperWorker
  _stepperWorkerMap.insert( std::map<int, StepperWorker>::value_type ( id, sw ));
}

void AnimationStore::addLedWorker(LedWorker lw){
  int id = lw.getId();
  // cannot use operator[] since we do not have an empty constructor or StepperWorker
  _ledWorkerMap.insert( std::map< int, LedWorker >::value_type ( id, lw ));
}

bool AnimationStore::_init_to_calibrating(){
  return _getCurrentAnimation().containsMotorFrames();
}

bool AnimationStore::_calibrate_to_active(){
  // return true if all calibrations are finished
  for (std::map<int, StepperWorker>::iterator it = _stepperWorkerMap.begin() ;
  it != _stepperWorkerMap.end(); ++it) {
    StepperWorker sw = it->second;
    if (sw.getState() != CALIBRATION_FINISHED){
      return false;
    }
  }
  return true;
}

void AnimationStore::_entry_calibrating(){
  printf("### Proceeding to state ANIMATION_CALIBRATING. ###\n");
  _startTime = millis(); // reset time
  for (std::map<int, StepperWorker>::iterator it = _stepperWorkerMap.begin() ;
  it != _stepperWorkerMap.end(); ++it) {
    StepperWorker sw = it->second;
    sw.startCalibration();
  }
}

void AnimationStore::_action_calibrating(){

  _elapsedTime = millis() - _startTime;

  for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
    StepperWorker sw = it->second;
    sw.loop(_elapsedTime);
  }
}

bool AnimationStore::_init_to_active(){
  return _animation.size() > 0 && (int) _animation.size() > _currentAnimationId &&
  ! _animation[_currentAnimationId].containsMotorFrames();
}

void AnimationStore::_entry_active(){
  printf("### No motor frames. Proceeding directly to state ANIMATION_ACTIVE. ###\n");
  _startTime = millis(); // reset time
  for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
    StepperWorker sw = it->second;
    sw.startAnimation();
  }
}

bool AnimationStore::_active_to_finish(){
  return _getCurrentAnimation().isAnimationFinished();
}

void AnimationStore::_entry_finished(){
  // choose next animation based on strategy
  if (_strategy == SINGLE) {
    // stop after first animation
    _currentAnimationId = -1;
  }
  else {
    _currentAnimationId++;
    if (_currentAnimationId >= getNumAnimations()) {
      if (_strategy_repeat){
        // restart animation
        _currentAnimationId = 0;
      }
      else {
        // stop animation
        _currentAnimationId = -1;
      }
    }
  }
}

void AnimationStore::_action_finished(){
  if (_currentAnimationId == -1){
    //TODO: do things to stop animation: go dark, stop steppers, ...
  }
}

bool AnimationStore::_finish_to_calibrating(){
  // continue if we have a valid id
  printf("### Proceeding with Animation %d ###.\n", _currentAnimationId);
  return _currentAnimationId >= 0;
}

void AnimationStore::_action_active(){

  _elapsedTime = millis() - _startTime;

  if (_getCurrentAnimation().needsTargetFrameUpdate(_elapsedTime)) {
    vector<KeyFrame> kfs = _getCurrentAnimation().getNextTargetKeyFrames(_elapsedTime);
    for (vector<KeyFrame>::iterator kf_it = kfs.begin(); kf_it != kfs.end(); kf_it++) {
      KeyFrame kf = *kf_it;

      bool keyFrameHandled = false;
      // check whether this is an update for a stepper worker
      auto sit =_stepperWorkerMap.find(kf_it->getId());
      if(sit != _stepperWorkerMap.end()) {
        StepperWorker sw = sit->second;
        sw.updateTargetKeyFrame(_elapsedTime, kf);
        keyFrameHandled = true;
   }

      // check whether this is an update for a stepper worker
      auto lit =_ledWorkerMap.find(kf_it->getId());
      if(lit != _ledWorkerMap.end()) {
        LedWorker lw = lit->second;
        lw.updateTargetKeyFrame(_elapsedTime, kf);
        keyFrameHandled = true;
      }
      if (!keyFrameHandled){
        printf("### WARNING. KeyFrame id did not match any worker: %d ###.\n", kf_it->getId());
	  }
    }
  }

  for (auto it = _stepperWorkerMap.begin(); it != _stepperWorkerMap.end(); ++it) {
    StepperWorker sw = it->second;
    sw.loop(_elapsedTime);
  }

  bool needLedUpdate = false;

  for (auto it = _ledWorkerMap.begin(); it != _ledWorkerMap.end(); ++it) {
    LedWorker lw = it->second;
    lw.loop(_elapsedTime);
    if (lw.needsUpdate()){
      needLedUpdate = true;
    }
  }

  if (needLedUpdate) {
    for (auto it = _ledWorkerMap.begin(); it != _ledWorkerMap.end(); ++it) {
      LedWorker lw = it->second;
      RGB rgbColor = lw.getColorForUpdate();
      _tlc.setLED(lw.getId(), rgbColor.red(), rgbColor.green(), rgbColor.blue());
    }
    _tlc.write();
  }

}



AnimationStore::AnimationStore()
: FiniteStateMachine (NUM_ANIMATION_STATES, ANIMATION_INIT, *this),
_currentAnimationId(-1), _elapsedTime(0)
{
  // LED test: leds turn red and black again one after the other
  Animation led_test1;
  addAnimation(led_test1);

  led_test1.addKeyFrames({
    {LED1BOT, 500,  RED, 100},
    {LED1TOP, 500,  BLACK, 100},

    {LED1BOT, 1000, BLACK, 100},
    {LED1TOP, 1000, RED, 100},
    {LED2BOT, 1000, BLACK, 100},

    {LED1TOP, 1500, BLACK, 100},
    {LED2BOT, 1500, RED, 100},
    {LED2TOP, 1500, BLACK, 100},

    {LED2BOT, 2000, BLACK, 100},
    {LED2TOP, 2000, RED, 100},
    {LED3BOT, 2000, BLACK, 100},

    {LED2TOP, 2500, BLACK, 100},
    {LED3BOT, 2500, RED, 100},
    {LED3TOP, 2500, BLACK, 100},

    {LED3BOT, 3000, BLACK, 100},
    {LED3TOP, 3000, RED, 100},
    {LED4BOT, 3000, BLACK, 100},

    {LED3TOP, 3500, BLACK, 100},
    {LED4BOT, 3500, RED, 100},
    {LED4TOP, 3500, BLACK, 100},

    {LED4BOT, 4000, BLACK, 100},
    {LED4TOP, 4000, RED, 100},

    {LED4TOP, 4500, BLACK, 100},
  });

  // LED test: all LEDs change colors at the same time
  Animation led_test2;
  addAnimation(led_test2);

  led_test2.addKeyFrames({
    {LED1BOT, 3000, RED, 100},
    {LED1TOP, 3000, RED, 100},
    {LED2BOT, 3000, RED, 100},
    {LED2TOP, 3000, RED, 100},
    {LED3BOT, 3000, RED, 100},
    {LED3TOP, 3000, RED, 100},
    {LED4BOT, 3000, RED, 100},
    {LED4TOP, 3000, RED, 100},

    {LED1BOT, 6000, GREEN, 100},
    {LED1TOP, 6000, GREEN, 100},
    {LED2BOT, 6000, GREEN, 100},
    {LED2TOP, 6000, GREEN, 100},
    {LED3BOT, 6000, GREEN, 100},
    {LED3TOP, 6000, GREEN, 100},
    {LED4BOT, 6000, GREEN, 100},
    {LED4TOP, 6000, GREEN, 100},

    {LED1BOT, 9000, BLUE, 100},
    {LED1TOP, 9000, BLUE, 100},
    {LED2BOT, 9000, BLUE, 100},
    {LED2TOP, 9000, BLUE, 100},
    {LED3BOT, 9000, BLUE, 100},
    {LED3TOP, 9000, BLUE, 100},
    {LED4BOT, 9000, BLUE, 100},
    {LED4TOP, 9000, BLUE, 100},

    {LED1BOT, 12000, RED, 0},
    {LED1TOP, 12000, RED, 0},
    {LED2BOT, 12000, RED, 0},
    {LED2TOP, 12000, RED, 0},
    {LED3BOT, 12000, RED, 0},
    {LED3TOP, 12000, RED, 0},
    {LED4BOT, 12000, RED, 0},
    {LED4TOP, 12000, RED, 0},

  });

  Animation a1;
  addAnimation(a1);

  a1.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER1, 2000, 0},
    {STEPPER1, 9000, 2600},
    {STEPPER1, 12000, 2600},
    {STEPPER1, 26000, 10}
  });

  a1.addKeyFrames({
    {STEPPER2,0, 0},
    {STEPPER2,3000, 0},
    {STEPPER2,10000, 2800},
    {STEPPER2,13000, 2800},
    {STEPPER2,27000, 10},
  });

  a1.addKeyFrames({
    {LED1TOP, 0,    YELLOW, 0},
    {LED1TOP, 1500, YELLOW, 50},
    {LED1TOP, 2000, YELLOW, 50},
    {LED1TOP, 9000, RED, 100},
    {LED1TOP, 9500, RED, 100},
    {LED1TOP, 11000, BLUE, 100},
    {LED1TOP, 12000, BLUE, 100},
    {LED1TOP, 26000, GREEN, 50},
    {LED1TOP, 27000, GREEN, 50},
    {LED1TOP, 28000, YELLOW, 50},
    {LED1TOP, 33000, YELLOW, 0},
  });

  a1.addKeyFrames({
    {LED1BOT, 0,    YELLOW, 0},
    {LED1BOT, 3500, YELLOW, 50},
    {LED1BOT, 4000, YELLOW, 50},
    {LED1BOT, 11000, RED, 100},
    {LED1BOT, 11500, RED, 100},
    {LED1BOT, 13000, BLUE, 100},
    {LED1BOT, 14000, BLUE, 100},
    {LED1BOT, 28000, GREEN, 50},
    {LED1BOT, 29000, GREEN, 50},
    {LED1BOT, 30000, YELLOW, 50},
    {LED1BOT, 35000, YELLOW, 0},
  });

  a1.addKeyFrames({
    {LED4TOP, 0,    YELLOW, 0},
    {LED4TOP, 1000, YELLOW, 0},
    {LED4TOP, 2500, YELLOW, 50},
    {LED4TOP, 3000, YELLOW, 50},
    {LED4TOP, 11000, RED, 100},
    {LED4TOP, 11500, RED, 100},
    {LED4TOP, 13000, BLUE, 100},
    {LED4TOP, 14000, BLUE, 100},
    {LED4TOP, 28000, GREEN, 50},
    {LED4TOP, 29000, GREEN, 50},
    {LED4TOP, 30000, YELLOW, 50},
    {LED4TOP, 35000, YELLOW, 50},
  });

  a1.addKeyFrames({
    {LED4BOT, 0,    YELLOW, 0},
    {LED4BOT, 1000,    YELLOW, 0},
    {LED4BOT, 4500, YELLOW, 50},
    {LED4BOT, 5000, YELLOW, 50},
    {LED4BOT, 14000, RED, 100},
    {LED4BOT, 13500, RED, 100},
    {LED4BOT, 15000, BLUE, 100},
    {LED4BOT, 16000, BLUE, 100},
    {LED4BOT, 30000, GREEN, 50},
    {LED4BOT, 31000, GREEN, 50},
    {LED4BOT, 32000, YELLOW, 50},
    {LED4BOT, 37000, YELLOW, 0},
  });


  //animation 2
  Animation a2;
  addAnimation(a2);
  a2.addKeyFrames(
    {
      {STEPPER1, 0, 0},
      {STEPPER4, 0, 0},
      {STEPPER4, 2000, 0},
      {STEPPER1, 4000, 3000},
      {STEPPER4, 6000, 3000},
      {STEPPER1, 8000, 0},
      {STEPPER4, 10000, 0},
    });

    a2.addKeyFrames({
      {LED1TOP, 400,  WHITE, 100},
      {LED1TOP, 1000, BLACK, 0},
      {LED1TOP, 2300, BLACK, 0},
      {LED1TOP, 2400, WHITE, 100},
      {LED1TOP, 3000, BLACK, 0},
      {LED1TOP, 20000, WHITE, 100},
      {LED1TOP, 22000, BLACK, 0},
    });

    a2.addKeyFrames({
      {LED1BOT, 200,  WHITE, 100},
      {LED1BOT, 800, BLACK, 0},
      {LED1BOT, 2000,  BLACK, 0},
      {LED1BOT, 2200,  WHITE, 100},
      {LED1BOT, 2800, BLACK, 0},
      {LED1BOT, 20000, WHITE, 100},
      {LED1BOT, 22000, BLACK, 0},
    });

    a2.addKeyFrames({
      {LED4TOP, 500,  WHITE, 100},
      {LED4TOP, 1000, BLACK, 0},
      {LED4TOP, 3600,  BLACK, 0},
      {LED4TOP, 3700,  WHITE, 100},
      {LED4TOP, 4100, BLACK, 0},
      {LED4TOP, 20000, WHITE, 100},
      {LED4TOP, 22000, BLACK, 0},
    });

    a2.addKeyFrames({
      {LED4BOT, 300,  WHITE, 100},
      {LED4BOT, 800, BLACK, 0},
      {LED4BOT, 3500,  BLACK, 0},
      {LED4BOT, 3600,  WHITE, 100},
      {LED4BOT, 4200, BLACK, 0},
      {LED4BOT, 20000, WHITE, 100},
      {LED4BOT, 22000, BLACK, 0},
    });
  }
