#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>
#include "Adafruit_TLC5947.h"

#include "KeyFrameStepper.h"
#include "KeyFrameRgbLED.h"

#include "Animation1Frames.h"
#include "RGB.h"

/*************************
    create LED objects
 *************************/

#define LED_DATA   4
#define LED_CLOCK  5
#define LED_LATCH  6
// these are hardwired in the lib using fast data write
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, LED_CLOCK, LED_DATA, LED_LATCH);

KeyFrameRgbLED rgb1o = KeyFrameRgbLED (4, rgb1u_kf, rgb1o_num);
KeyFrameRgbLED rgb1u = KeyFrameRgbLED (5, rgb1o_kf, rgb1u_num);
/*KeyFrameRgbLED rgb2o = KeyFrameRgbLED (7, rgb2u_kf, 3);
  KeyFrameRgbLED rgb2u = KeyFrameRgbLED (8, rgb2o_kf, 3);
  KeyFrameRgbLED rgb3o = KeyFrameRgbLED (5, rgb3u_kf, 3);
  KeyFrameRgbLED rgb3u = KeyFrameRgbLED (6, rgb3o_kf, 3);
*/
KeyFrameRgbLED rgb4o = KeyFrameRgbLED (6, rgb4u_kf, rgb4o_num);
KeyFrameRgbLED rgb4u = KeyFrameRgbLED (7, rgb4o_kf, rgb4u_num);

/*************************
    create Stepper objects
 *************************/
// two stepper motors one on each port
//Adafruit_MotorShield AFMS_a = Adafruit_MotorShield(0x60);
Adafruit_MotorShield AFMS_b = Adafruit_MotorShield(0x61);

Adafruit_StepperMotor *steppermotor1 = AFMS_b.getStepper(200, 2);
//Adafruit_StepperMotor *steppermotor2 = AFMS_a.getStepper(200, 2);
//Adafruit_StepperMotor *steppermotor3 = AFMS_a.getStepper(200, 1);
Adafruit_StepperMotor *steppermotor4 = AFMS_b.getStepper(200, 1);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep1() {
  steppermotor1->onestep(FORWARD, INTERLEAVE);
}
void backwardstep1() {
  steppermotor1->onestep(BACKWARD, INTERLEAVE);
}
AccelStepper astepper1(forwardstep1, backwardstep1); // use functions to step
/*
  void forwardstep2() {
  steppermotor2->onestep(FORWARD, DOUBLE);
  }
  void backwardstep2() {
  steppermotor2->onestep(BACKWARD, DOUBLE);
  }
  AccelStepper astepper2(forwardstep2, backwardstep2); // use functions to step

  void forwardstep3() {
  steppermotor3->onestep(FORWARD, INTERLEAVE);
  }
  void backwardstep3() {
  steppermotor3->onestep(BACKWARD, INTERLEAVE);
  }
  AccelStepper astepper3(forwardstep3, backwardstep3); // use functions to step
*/
void forwardstep4() {
  steppermotor4->onestep(FORWARD, INTERLEAVE);
}
void backwardstep4() {
  steppermotor4->onestep(BACKWARD, INTERLEAVE);
}
AccelStepper astepper4(forwardstep4, backwardstep4); // use functions to step

KeyFrameStepper  kfstepper1 = KeyFrameStepper(steppermotor1, astepper1, 1, motor1_kf, motor1_num, 53, true);
//KeyFrameStepper  kfstepper2 = KeyFrameStepper(steppermotor2, astepper2, 2, motor2_kf, 2, 49, false);
//KeyFrameStepper  kfstepper3 = KeyFrameStepper(steppermotor3, astepper3, 3, motor3_kf, 2, 51, false);
KeyFrameStepper  kfstepper4 = KeyFrameStepper(steppermotor4, astepper4, 4, motor4_kf, motor4_num, 47, true);

/************
    Setup
 ************/

void setup()
{
  Serial.begin(9600);
  Serial.println("start");

  //AFMS_a.begin();
  AFMS_b.begin();
  // Change the i2c clock to 400KHz
  TWBR = ((F_CPU / 400000l) - 16) / 2;

  //kfstepper1.init();
  //kfstepper2.init();
  //kfstepper3.init();
  kfstepper4.init();

  kfstepper1.calibrate();
  //kfstepper2.calibrate();
  //kfstepper3.calibrate();
  kfstepper4.calibrate();

  kfstepper1.start();
  //kfstepper2.start();
  //kfstepper3.start();
  kfstepper4.start();

  tlc.begin();
  //led.doFineSerialOutput(true);

  rgb1o.start();
  rgb1u.start();
  /*
    rgb2o.start();
    rgb2u.start();
    rgb3o.start();
    rgb3u.start();
  */
  rgb4o.start();
  rgb4u.start();
}

/************
    Loop
 ************/

void loop()
{
  kfstepper1.loop();
  //kfstepper2.loop();
  //kfstepper3.loop();
  kfstepper4.loop();


  rgb1o.loop();
  rgb1u.loop();
  //rgb2o.loop();
  //rgb2u.loop();
  //rgb3o.loop();
  //rgb3u.loop();
  rgb4o.loop();
  rgb4u.loop();

  if (rgb1o.needsUpdate() || rgb1u.needsUpdate() || rgb4o.needsUpdate() || rgb4u.needsUpdate()) {
    updateLEDs(tlc, rgb1o.getId(), rgb1o.getCurrentColor(), rgb1u.getId(), rgb1u.getCurrentColor(),
               rgb4o.getId(), rgb4o.getCurrentColor(), rgb4u.getId(), rgb4u.getCurrentColor());
  }

}

