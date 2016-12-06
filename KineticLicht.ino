#include <Adafruit_MotorShield.h>
#include "Adafruit_TLC5947.h"

#include "KeyFrameStepper.h"
#include "KeyFrameRgbLED.h"

// two stepper motors one on each port
//AF_Stepper motor1(200, 1);
//AF_Stepper motor2(200, 2);
Adafruit_MotorShield AFMS_a = Adafruit_MotorShield(0x61); 
Adafruit_MotorShield AFMS_b = Adafruit_MotorShield(0x61); 

Adafruit_StepperMotor *motor1 = AFMS_a.getStepper(200, 0);
Adafruit_StepperMotor *motor2 = AFMS_a.getStepper(200, 1);
Adafruit_StepperMotor *motor3 = AFMS_b.getStepper(200, 0);
Adafruit_StepperMotor *motor4 = AFMS_b.getStepper(200, 1);

#define data   4
#define clock   5
#define latch   6
#define oe  -1  // set to -1 to not use the enable pin (its optional)
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, clock, data, latch);



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
KeyFrameRgbLED       led1 = KeyFrameRgbLED (&tlc, 0, keyFramesRgb1, 3);

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
