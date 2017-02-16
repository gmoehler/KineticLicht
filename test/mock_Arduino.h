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

class Adafruit_TLC5947 {
 public:
  Adafruit_TLC5947();
  void begin(void);
  void setLED(int lednum, int r, int g, int b);
  void write(void);
};



#define INPUT_PULLUP 42
#define LOW 0
#define HIGH 1

unsigned long millis();
void delay(unsigned long ms);

void pinMode(int, int);
int digitalRead(int pin);

void test_triggerEndStop(bool isHit);

#endif
