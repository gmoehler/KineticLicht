#ifndef mock_arduino_h
#define mock_arduino_h

#include <stdio.h>

class AccelStepper {
public:
  AccelStepper();
  void runSpeed();
  void setSpeed(double speed);
  long currentPosition();
  void setCurrentPosition(long pos);

  double test_getSpeed();

private:
  double _speed;
};

#define INPUT_PULLUP 42
#define LOW 0
#define HIGH 1

unsigned long millis();
void delay(unsigned long ms);

void pinMode(int, int);
int digitalRead(int pin);

void test_triggerEndStop();
int test_getNumEndpointResets();

#endif
