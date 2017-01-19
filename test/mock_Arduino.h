#include <sys/timeb.h>

class Adafruit_StepperMotor {
public:
  void release();
};

class AccelStepper {
public:
  void runSpeed();
  void setSpeed(double speed);
  long currentPosition();
  void setCurrentPosition(long pos);
};

#define INPUT_PULLUP 42
#define LOW 0
#define HIGH 1

unsigned long millis();
void delay(unsigned long ms);

void pinMode(int, int);
int digitalRead(int pin);

void init_mock();
