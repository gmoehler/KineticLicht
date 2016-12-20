#include <Adafruit_MotorShield.h>
#include "Adafruit_TLC5947.h"

#include "KeyFrameStepper.h"
#include "KeyFrameRgbLED.h"

#include <AccelStepper.h>


// two stepper motors one on each port
Adafruit_MotorShield AFMS_a = Adafruit_MotorShield(0x60); 
Adafruit_MotorShield AFMS_b = Adafruit_MotorShield(0x61); 

Adafruit_StepperMotor *steppermotor1 = AFMS_a.getStepper(200, 1);
Adafruit_StepperMotor *steppermotor2 = AFMS_a.getStepper(200, 2);
Adafruit_StepperMotor *steppermotor3 = AFMS_b.getStepper(200, 1);
Adafruit_StepperMotor *steppermotor4 = AFMS_b.getStepper(200, 2);

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



KeyFrameRgbLED      rgb1o = KeyFrameRgbLED (&tlc, 0, rgb1u_kf, 3);
KeyFrameRgbLED      rgb1u = KeyFrameRgbLED (&tlc, 1, rgb1o_kf, 3);
KeyFrameRgbLED      rgb2o = KeyFrameRgbLED (&tlc, 2, rgb2u_kf, 3);
KeyFrameRgbLED      rgb2u = KeyFrameRgbLED (&tlc, 3, rgb2o_kf, 3);
KeyFrameRgbLED      rgb3o = KeyFrameRgbLED (&tlc, 4, rgb3u_kf, 3);
KeyFrameRgbLED      rgb3u = KeyFrameRgbLED (&tlc, 5, rgb3o_kf, 3);
KeyFrameRgbLED      rgb4o = KeyFrameRgbLED (&tlc, 6, rgb4u_kf, 3);
KeyFrameRgbLED      rgb4u = KeyFrameRgbLED (&tlc, 7, rgb4o_kf, 3);
// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep1() {  
  steppermotor1->onestep(FORWARD, DOUBLE);
}
void backwardstep1() {  
  steppermotor1->onestep(BACKWARD, DOUBLE);
}
AccelStepper astepper1(forwardstep1, backwardstep1); // use functions to step

void forwardstep2() {  
  steppermotor2->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {  
  steppermotor2->onestep(BACKWARD, DOUBLE);
}
AccelStepper astepper2(forwardstep2, backwardstep2); // use functions to step

void forwardstep3() {  
  steppermotor3->onestep(FORWARD, DOUBLE);
}
void backwardstep3() {  
  steppermotor3->onestep(BACKWARD, DOUBLE);
}
AccelStepper astepper3(forwardstep3, backwardstep3); // use functions to step

void forwardstep4() {  
  steppermotor4->onestep(FORWARD, DOUBLE);
}
void backwardstep4() {  
  steppermotor4->onestep(BACKWARD, DOUBLE);
}
AccelStepper astepper4(forwardstep4, backwardstep4); // use functions to step

KeyFrameStepper  kfstepper1 = KeyFrameStepper(steppermotor1, astepper1, 1, motor1_kf, 2, 47);
KeyFrameStepper  kfstepper2 = KeyFrameStepper(steppermotor2, astepper2, 2, motor2_kf, 2, 49);
KeyFrameStepper  kfstepper3 = KeyFrameStepper(steppermotor3, astepper3, 3, motor3_kf, 2, 51);
KeyFrameStepper  kfstepper4 = KeyFrameStepper(steppermotor4, astepper4, 4, motor4_kf, 2, 53);


//RgbLED led = RgbLED( &tlc, 4);

void setup()
{
  Serial.begin(9600);
  Serial.println("start");

  AFMS_a.begin();
  AFMS_b.begin();
  TWBR = ((F_CPU /400000l) - 16) / 2; // Change the i2c clock to 400KHz
  
  kfstepper1.start();
  kfstepper2.start();
  kfstepper3.start();
  kfstepper4.start();

/*  
  astepper1.setSpeed(50);
  astepper2.setSpeed(100);
  astepper3.setSpeed(200);
  astepper4.setSpeed(4000);
*/
/*
  astepper1.moveTo(300);
  astepper2.moveTo(500);
  astepper3.moveTo(200);
  astepper4.moveTo(300);
*/
  tlc.begin();
  //led.doFineSerialOutput(true);
  
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

 if (kfstepper1.isEndStopHit()){
  Serial.println("1 Stop");
 }
 if (kfstepper2.isEndStopHit()){
  Serial.println("2 Stop");
 }
 if (kfstepper3.isEndStopHit()){
  Serial.println("3 Stop");
 }
 if (kfstepper4.isEndStopHit()){
  Serial.println("4 Stop");
 }

  kfstepper3.calibrate();

/*  
  astepper1.runSpeed();
  astepper2.runSpeed();
  astepper3.runSpeed();
  astepper4.runSpeed();
*/ 
  //rgb1o.loop();
  //rgb1u.loop();
  //rgb2o.loop();
  //rgb2u.loop();
  //rgb3o.loop();
  //rgb3u.loop();
  //rgb4o.loop();
  //rgb4u.loop();
}
