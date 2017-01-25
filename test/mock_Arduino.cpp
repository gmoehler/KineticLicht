#include "mock_Arduino.h"

AccelStepper::AccelStepper() : _speed(-999) {

}

void AccelStepper::runSpeed(){
}

void AccelStepper::setSpeed(double speed){
  printf("setSpeed: %.2f\n", speed);
  _speed = speed;
}

double AccelStepper::test_getSpeed(){
  return _speed;
}

long AccelStepper::currentPosition(){
  return 0L;
}

void AccelStepper::setCurrentPosition(long pos){
  printf("setCurPos: %ld\n", pos);
}

long _testCurrentTime=0L;
unsigned long millis() {
  _testCurrentTime+=100;
  printf("millis() %ld\n",_testCurrentTime );
  return _testCurrentTime;
}

void delay(unsigned long ms) {
  printf("Waiting for %ld ms.", ms);
}

void pinMode(int pin, int mode){
  printf("Set pinMode for %d to %d", pin, mode);
}

bool _testIsEndStopHit = false;
int _testCountEndStopHits = 0;
int _testNumEndStopResets=0;
int digitalRead(int pin){
  if (_testIsEndStopHit){
    printf("Endstop hit: ...%d\n", _testCountEndStopHits);
  }
  else {
    printf("Endstop not hit.\n");
  }
  // automatically reset endstop hit after 10 calls
  if (_testIsEndStopHit){
    _testCountEndStopHits++;
    millis();
    if (_testCountEndStopHits > 10){
      printf("RESET ENDSTOP_HIT\n");
      _testIsEndStopHit=false;
      _testCountEndStopHits=0;
      _testNumEndStopResets++;
    }
  }
  return _testIsEndStopHit ? LOW : HIGH;
}

void test_triggerEndStop(){
  printf("Triggering endstop...\n");
  _testIsEndStopHit = true;
  _testCountEndStopHits=0;
}

int test_getNumEndpointResets(){
  return _testNumEndStopResets;
}
