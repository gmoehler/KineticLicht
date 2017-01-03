#include <Arduino.h>
#include <ArduinoSTL.h>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

#include "Adafruit_TLC5947.h"

#include "RGB.h"
#include "AnimationStore.h"
#include "StepperWorker.h"
#include "LedWorker.h"

enum AnimationState {ANIMATION_ACTIVE, ANIMATION_INIT, ANIMATION_CALIBRATING, ANIMATION_FINISHED, ANIMATION_ERROR};

/*************************
    create LED objects
 *************************/

#define LED_DATA   4
#define LED_CLOCK  5
#define LED_LATCH  6
// these are hard-wired in the lib using fast data write
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, LED_CLOCK, LED_DATA, LED_LATCH);

LedWorker rgb1o = LedWorker (4);
LedWorker rgb1u = LedWorker (5);
/*LedWorker rgb2o = LedWorker (7);
  LedWorker rgb2u = LedWorker (8);
  LedWorker rgb3o = LedWorker (5);
  LedWorker rgb3u = LedWorker (6);
*/
LedWorker rgb4o = LedWorker (6);
LedWorker rgb4u = LedWorker (7);

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
  steppermotor2->onestep(FORWARD, INTERLEAVE);
  }
  void backwardstep2() {
  steppermotor2->onestep(BACKWARD, INTERLEAVE);
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


StepperWorker  sworker1 = StepperWorker(steppermotor1, astepper1, 1, 53, true);
//StepperWorker  sworker2 = StepperWorker(steppermotor2, astepper2, 2, 49, false);
//StepperWorker  sworker3 = StepperWorker(steppermotor3, astepper3, 3, 51, false);
StepperWorker  sworker4 = StepperWorker(steppermotor4, astepper4, 4, 47, true);

/************
    Setup
 ************/

long startTime = 0;
long elapsedTime = 0;
AnimationState state = ANIMATION_INIT;

AnimationStore animationStore = AnimationStore();
int animationID = 0;
Animation animation = animationStore.getAnimation(animationID);

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
}

/************
    Loop
 ************/

void loop()
{
  switch (state) {
    case ANIMATION_ACTIVE:

      elapsedTime = millis() - startTime;

      if (animation.isAnimationFinished()) {
        state = ANIMATION_FINISHED;
        break;
      }

      // update workers with new key frames
      if (animation.hasNextTargetKeyFrame(elapsedTime)) {
        vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(elapsedTime);
        for (vector<KeyFrame>::iterator kf_it = kfs.begin(); kf_it != kfs.end(); kf_it++) {
          KeyFrame kf = *kf_it;
          switch (kf_it->getId()) {
            case STEPPER1:
              sworker1.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case STEPPER4:
              sworker4.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case LED1TOP:
              rgb1o.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case LED1BOT:
              rgb1u.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case LED4TOP:
              rgb4o.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case LED4BOT:
              rgb4u.updateTargetKeyFrame(elapsedTime, kf);
              break;
          }
        }

        // do the work
        sworker1.loop(elapsedTime);
        //sworker2.loop(elapsedTime);
        //sworker3.loop(elapsedTime);
        sworker4.loop(elapsedTime);

        rgb1o.loop(elapsedTime);
        rgb1u.loop(elapsedTime);
        //rgb2o.loop(elapsedTime);
        //rgb2u.loop(elapsedTime);
        //rgb3o.loop(elapsedTime);
        //rgb3u.loop(elapsedTime);
        rgb4o.loop(elapsedTime);
        rgb4u.loop(elapsedTime);

        if (rgb1o.needsUpdate() || rgb1u.needsUpdate() || rgb4o.needsUpdate() || rgb4u.needsUpdate()) {
          updateLEDs(tlc, rgb1o.getId(), rgb1o.getColorForUpdate(), rgb1u.getId(), rgb1u.getColorForUpdate(),
                     rgb4o.getId(), rgb4o.getColorForUpdate(), rgb4u.getId(), rgb4u.getColorForUpdate());
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

      animationID++;
      if (animationID >= animationStore.getNumAnimations()) {
        animationID = 0;
      }
      animation = animationStore.getAnimation(animationID);
      delay(1000);
      state = ANIMATION_CALIBRATING;
      break;
  }
}

