#include <Adafruit_MotorShield.h>
#include "Adafruit_TLC5947.h"

#include "KeyFrameStepper.h"
#include "KeyFrameRgbLED.h"

#include <AccelStepper.h>


// two stepper motors one on each port
Adafruit_MotorShield AFMS_a(0x60); 
Adafruit_MotorShield AFMS_b(0x61); 

Adafruit_StepperMotor *motor1 = AFMS_a.getStepper(200, 1);
Adafruit_StepperMotor *motor2 = AFMS_a.getStepper(200, 2);
Adafruit_StepperMotor *motor3 = AFMS_b.getStepper(200, 1);
Adafruit_StepperMotor *motor4 = AFMS_b.getStepper(200, 2);

#define data   4
#define clock  5
#define latch  6
#define oe  -1  // set to -1 to not use the enable pin (its optional)
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, clock, data, latch);

KeyFrame motor1_kf[2] {
  {5000, 400},
  {7000, 200}
};

KeyFrame motor2_kf[2] {
  {5000, 400},
  {7000, 200}
};

KeyFrame motor3_kf[2] {
  {5000, 400},
  {7000, 200}
};

KeyFrame motor4_kf[2] {
  {5000, 800},
  {7000, 30}
};


KeyFrameRgb rgb1o_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};

KeyFrameRgb rgb1u_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb2o_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb2u_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb3o_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb3u_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb4o_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb4u_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};


KeyFrameStepper  stepper1 = KeyFrameStepper(motor1, 1, motor1_kf, 2);
KeyFrameStepper  stepper2 = KeyFrameStepper(motor2, 2, motor2_kf, 2);
KeyFrameStepper  stepper3 = KeyFrameStepper(motor3, 3, motor3_kf, 2);
KeyFrameStepper  stepper4 = KeyFrameStepper(motor4, 4, motor4_kf, 2);
KeyFrameRgbLED      rgb1o = KeyFrameRgbLED (&tlc, 0, rgb1u_kf, 3);
KeyFrameRgbLED      rgb1u = KeyFrameRgbLED (&tlc, 1, rgb1o_kf, 3);
KeyFrameRgbLED      rgb2o = KeyFrameRgbLED (&tlc, 2, rgb2u_kf, 3);
KeyFrameRgbLED      rgb2u = KeyFrameRgbLED (&tlc, 3, rgb2o_kf, 3);
KeyFrameRgbLED      rgb3o = KeyFrameRgbLED (&tlc, 4, rgb3u_kf, 3);
KeyFrameRgbLED      rgb3u = KeyFrameRgbLED (&tlc, 5, rgb3o_kf, 3);
KeyFrameRgbLED      rgb4o = KeyFrameRgbLED (&tlc, 6, rgb4u_kf, 3);
KeyFrameRgbLED      rgb4u = KeyFrameRgbLED (&tlc, 7, rgb4o_kf, 3);

RgbLED led = RgbLED( &tlc, 4);

AccelStepper astepper1(motor1->onestep(FORWARD, SINGLE), motor1->onestep(BACKWARD, SINGLE)); // use functions to step
AccelStepper astepper2(motor2->onestep(FORWARD, SINGLE), motor2->onestep(BACKWARD, SINGLE)); // use functions to step
AccelStepper astepper3(motor3->onestep(FORWARD, SINGLE), motor3->onestep(BACKWARD, SINGLE)); // use functions to step
AccelStepper astepper4(motor4->onestep(FORWARD, SINGLE), motor4->onestep(BACKWARD, SINGLE)); // use functions to step


void setup()
{
  Serial.begin(9600);
  Serial.println("start");
  
  AFMS_a.begin();
  AFMS_b.begin();
  
  stepper1.start();
  stepper2.start();
  stepper3.start();
  stepper4.start();

  astepper1.setSpeed(50);
  astepper2.setSpeed(100);
  astepper3.setSpeed(200);
  astepper4.setSpeed(50);

  astepper1.moveTo(300);
  astepper2.moveTo(500);
  astepper3.moveTo(200);
  astepper4.moveTo(300);

  tlc.begin();
  led.doFineSerialOutput(true);
  
  rgb1o.start();
  rgb1u.start();
  rgb2o.start();
  rgb2u.start();
  rgb3o.start();
  rgb3u.start();
  rgb4o.start();
  rgb4u.start();
}

void loop()
{
 //stepper1.loop();
 //stepper2.loop();
 //stepper3.loop();
 //stepper4.loop();

  astepper1.runSpeed();
  astepper2.runSpeed();
  astepper3.runSpeed();
  astepper4.runSpeed();

  //rgb1o.loop();
  //rgb1u.loop();
  //rgb2o.loop();
  //rgb2u.loop();
  //rgb3o.loop();
  //rgb3u.loop();
  //rgb4o.loop();
  //rgb4u.loop();
}
