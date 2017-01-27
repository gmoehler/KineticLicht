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
  printf("millis() %ld\n",_testCurrentTime );
  _testCurrentTime+=100;
  return _testCurrentTime;
}

void delay(unsigned long ms) {
  printf("Waiting for %ld ms.", ms);
    _testCurrentTime+=ms;
}

void pinMode(int pin, int mode){
  printf("Set pinMode for %d to %d", pin, mode);
}

bool _testIsEndStopHit = false;
int digitalRead(int pin){
  if (_testIsEndStopHit){
    printf("Endstop hit.\n");
  }
  else {
    printf("Endstop not hit.\n");
  }
  return _testIsEndStopHit ? LOW : HIGH;
}

void test_triggerEndStop(bool isHit){
  if (isHit) {
    printf("Triggering endstop...\n");
  }
  else {
    printf("Untriggering endstop...\n");
  }
  _testIsEndStopHit = isHit;
}
