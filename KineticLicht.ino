#include <Arduino.h>
#include <ArduinoSTL.h>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

#include "Adafruit_TLC5947.h"

#include "StepperWorker.h"
#include "KeyFrameRgbLED.h"

#include "AnimationStore.h"
#include "RGB.h"

enum AnimationState {ANIMATION_ACTIVE, ANIMATION_INIT, ANIMATION_CALIBRATING, ANIMATION_FINISHED, ANIMATION_ERROR};
enum Activators {STEPPER1, STEPPER2, STEPPER3, STEPPER4, LED1TOP, LED1BOT, LED2TOP, LED2BOT, LED3TOP, LED3BOT, LED4TOP, LED4BOT};

/*************************
    create LED objects
 *************************/

#define LED_DATA   4
#define LED_CLOCK  5
#define LED_LATCH  6
// these are hard-wired in the lib using fast data write
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, LED_CLOCK, LED_DATA, LED_LATCH);

AnimationStore animationStore = AnimationStore();
Animation a1 = animationStore.getAnimation(0);

vector<KeyFrameRgb> lkfs1 = a1.getRgbKeyframes(1, BOTTOM);
KeyFrameRgbLED rgb1o = KeyFrameRgbLED (4, lkfs1);
vector<KeyFrameRgb> lkfs2 = a1.getRgbKeyframes(1, TOP);
KeyFrameRgbLED rgb1u = KeyFrameRgbLED (5, lkfs2);
/*KeyFrameRgbLED rgb2o = KeyFrameRgbLED (7, rgb2u_kf, 3);
  KeyFrameRgbLED rgb2u = KeyFrameRgbLED (8, rgb2o_kf, 3);
  KeyFrameRgbLED rgb3o = KeyFrameRgbLED (5, rgb3u_kf, 3);
  KeyFrameRgbLED rgb3u = KeyFrameRgbLED (6, rgb3o_kf, 3);
*/
vector<KeyFrameRgb> lkfs7 = a1.getRgbKeyframes(1, BOTTOM);
KeyFrameRgbLED rgb4o = KeyFrameRgbLED (6, lkfs7);
vector<KeyFrameRgb> lkfs8 = a1.getRgbKeyframes(1, BOTTOM);
KeyFrameRgbLED rgb4u = KeyFrameRgbLED (7, lkfs8);

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

vector<KeyFrame> mkfs1 =  a1.getMotorKeyframes(1);
StepperWorker  sworker1 = StepperWorker(steppermotor1, astepper1, 1, 53, true);
//StepperWorker  sworker2 = StepperWorker(steppermotor2, astepper2, 2, 49, false);
//StepperWorker  sworker3 = StepperWorker(steppermotor3, astepper3, 3, 51, false);
vector<KeyFrame> mkfs4 =  a1.getMotorKeyframes(4);
StepperWorker  sworker4 = StepperWorker(steppermotor4, astepper4, 4, 47, true);

long startTime = 0;
long elapsedTime = 0;
AnimationState state = ANIMATION_INIT;

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

  startTime = millis();

  sworker1.init();
  //sworker2.init();
  //sworker3.init();
  sworker4.init();

  tlc.begin();

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

  switch (state) {
    case ANIMATION_ACTIVE:

      elapsedTime = millis() - startTime;

      if (a1.isAnimationFinished(elapsedTime)) {
        state = ANIMATION_FINISHED;
      }
      
      else {
        if (a1.hasNextTargetKeyFrame(elapsedTime)) {
          KeyFrame kf = a1.getNextTargetKeyFrame(elapsedTime);
          switch (kf.getId()) {
            case STEPPER1:
              sworker1.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case STEPPER4:
              sworker4.updateTargetKeyFrame(elapsedTime, kf);
              break;
          }
        }

        sworker1.loop(elapsedTime);
        //sworker2.loop(elapsedTime);
        //sworker3.loop(elapsedTime);
        sworker4.loop(elapsedTime);

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
      break;

    case ANIMATION_INIT:
      state = ANIMATION_CALIBRATING;
      break;

    case ANIMATION_CALIBRATING:
      sworker1.loopCalibration();
      //sworker2.loopCalibration();
      //sworker3.loopCalibration();
      sworker4.loopCalibration();

      if (sworker1.getState() == CALIBRATION_FINISHED && sworker4.getState() == CALIBRATION_FINISHED) {
        state = ANIMATION_ACTIVE;
      }
      break;

    case ANIMATION_FINISHED:
      break;
  }



}

