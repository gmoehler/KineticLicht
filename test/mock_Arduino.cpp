#include "mock_arduino.h"

void Adafruit_StepperMotor::release() {
}

void AccelStepper::runSpeed(){

}

void AccelStepper::setSpeed(double speed){

}
long AccelStepper::currentPosition(){
  return 0L;
}

void AccelStepper::setCurrentPosition(long pos){

}

timeb t_start;
unsigned long millis() {
  timeb t_now;
  ftime(&t_now);
  return (t_now.time  - t_start.time) * 1000 + (t_now.millitm - t_start.millitm);
}

void delay(unsigned long ms) {
  unsigned long start = millis();
  while(millis() - start < ms){}
}

void pinMode(int pin, int mode){

}

int digitalRead(int pin){
  return 1;
}

void init_mock() {
  ftime(&t_start);
}
