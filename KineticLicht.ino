#include <AFMotor.h>
#include "KeyFrameStepper.h"
#include "KeyFrameRgbLED.h"

// two stepper motors one on each port
AF_Stepper motor1(200, 1);
AF_Stepper motor2(200, 2);

int  ledRedPin = 44;
int  ledGreenPin = 45;
int  ledBluePin = 46;



KeyFrame keyFrames1[2] {
  {5000, 400},
  {7000, 200}
};

KeyFrame keyFrames2[2] {
  {5000, 400},
  {7000, 200}
};

KeyFrameRgb keyFramesRgb1[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};


KeyFrameStepper  stepper1 = KeyFrameStepper(motor1, keyFrames1, 2);
KeyFrameStepper  stepper2 = KeyFrameStepper(motor2, keyFrames2, 2);
KeyFrameRgbLED       led1 = KeyFrameRgbLED(ledRedPin, ledGreenPin, ledBluePin, keyFramesRgb1, 3);

void setup()
{
  Serial.begin(9600);
  Serial.println("start");
  
  stepper1.start();
  stepper2.start();
  led1.start();
}

void loop()
{
  stepper1.loop();
  stepper2.loop();
  led1.loop();
}
