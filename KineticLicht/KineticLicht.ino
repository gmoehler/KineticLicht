#include <Arduino.h>
#include <ArduinoSTL.h>
#include <map>
#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>
#include <IRremote.h>

#include "Adafruit_TLC5947.h"

#include "RGB.h"
#include "AnimationStore.h"
#include "StepperWorker.h"
#include "LedWorker.h"

enum AnimationState {ANIMATION_ACTIVE, ANIMATION_INIT, ANIMATION_CALIBRATING, ANIMATION_FINISHED};

std::map<long, int> create_NumberButtonMap();
void updateLEDs(Adafruit_TLC5947 tlc, LedWorker rgb1o, LedWorker rgb1u, LedWorker rgb2o, LedWorker rgb2u,
                LedWorker rgb3o, LedWorker rgb3u, LedWorker rgb4o, LedWorker rgb4u);

/*************************
    create LED objects
 *************************/

#define LED_DATA   4
#define LED_CLOCK  5
#define LED_LATCH  6
// these are hard-wired in the lib using fast data write
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, LED_CLOCK, LED_DATA, LED_LATCH);

LedWorker rgb1o = LedWorker (0);
LedWorker rgb1u = LedWorker (1);
LedWorker rgb2o = LedWorker (2);
LedWorker rgb2u = LedWorker (3);
LedWorker rgb3o = LedWorker (4);
LedWorker rgb3u = LedWorker (5);
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


StepperWorker  sworker1 = StepperWorker(astepper1, 1, 53, true);
//StepperWorker  sworker2 = StepperWorker(astepper2, 2, 49, false);
//StepperWorker  sworker3 = StepperWorker(astepper3, 3, 51, false);
StepperWorker  sworker4 = StepperWorker(astepper4, 4, 47, true);

/****************************
    Create IR remote objects
 ****************************/

int IR_RECV_PIN = 11;

IRrecv irrecv(IR_RECV_PIN);
decode_results irResults;
std::map<long, int> numberButtons = create_NumberButtonMap();

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

  tlc.begin(); // start LED PWM decoder

  irrecv.enableIRIn(); // Start the IR receiver
}

/************
    Loop
 ************/

void loop()
{
  /*
  // react on IR anmiation selection
  if (irrecv.decode(&irResults)) {
    // pressed a number button: select animation with that id
    int num = irResults.value;
    if (numberButtons.count(num) == 1 && num < animationStore.getNumAnimations()) {
      animation = animationStore.getAnimation(num);
      delay(1000);
      state = ANIMATION_CALIBRATING;
    }
    irrecv.resume(); // Receive the next value
  }
*/

  // work on states
  switch (state) {
    case ANIMATION_ACTIVE:

      elapsedTime = millis() - startTime;

      if (animation.isAnimationFinished()) {
        state = ANIMATION_FINISHED;
        break;
      }

      // update workers with new key frames
      if (animation.needsTargetFrameUpdate(elapsedTime)) {
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
            case LED2TOP:
              rgb2o.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case LED2BOT:
              rgb2u.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case LED3TOP:
              rgb3o.updateTargetKeyFrame(elapsedTime, kf);
              break;
            case LED3BOT:
              rgb3u.updateTargetKeyFrame(elapsedTime, kf);
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
        rgb2o.loop(elapsedTime);
        rgb2u.loop(elapsedTime);
        rgb3o.loop(elapsedTime);
        rgb3u.loop(elapsedTime);
        rgb4o.loop(elapsedTime);
        rgb4u.loop(elapsedTime);

        if (rgb1o.needsUpdate() || rgb1u.needsUpdate() || rgb2o.needsUpdate() || rgb2u.needsUpdate() ||
            rgb3o.needsUpdate() || rgb3u.needsUpdate() || rgb4o.needsUpdate() || rgb4u.needsUpdate()) {
          updateLEDs(tlc, rgb1o, rgb1u, rgb2o, rgb2u, rgb3o, rgb3u, rgb4o, rgb4u);
        }
      }
      break;

    case ANIMATION_INIT:
      // proceed to calibration
      if (animation.containsMotorFrames()){
        printf("### Proceeding to state ANIMATION_CALIBRATING. ###\n");
        sworker1.startCalibration();
        //sworker2.startCalibration();
        //sworker3.startCalibration();
        sworker4.startCalibration();

        state = ANIMATION_CALIBRATING;
      }
      else {
        printf("### No motor frames. Proceeding directly to state ANIMATION_ACTIVE. ###\n");
        elapsedTime = millis(); // reset time
        sworker1.startAnimation();
        //sworker2.startAnimation();
        //sworker3.startAnimation();
        sworker4.startAnimation();

        state = ANIMATION_ACTIVE;
      }
      break;

    case ANIMATION_CALIBRATING:
      sworker1.loop(elapsedTime);
      //sworker2.loop(elapsedTime);
      //sworker3.loop(elapsedTime);
      sworker4.loop(elapsedTime);

      if (sworker1.getState() == CALIBRATION_FINISHED && sworker4.getState() == CALIBRATION_FINISHED) {
        printf("### Proceeding to state ANIMATION_ACTIVE. ###\n");
        elapsedTime = millis(); // reset time
        sworker1.startAnimation();
        //sworker2.startAnimation();
        //sworker3.startAnimation();
        sworker4.startAnimation();

        state = ANIMATION_ACTIVE;
      }
      break;

    case ANIMATION_FINISHED:
      animationID++;
      if (animationID >= animationStore.getNumAnimations()) {
        animationID = 0;
      }
      printf("### Proceeding with Animation %d ###.\n", animationID);
      animation = animationStore.getAnimation(animationID);
      delay(1000);
      state = ANIMATION_CALIBRATING;
      break;
  }
}

std::map<long, int> create_NumberButtonMap()
{
  std::map<long, int> m;
  m[16738455] = 0;
  m[16724175] = 1;
  m[16718055] = 2;
  m[16743045] = 3;
  m[16716015] = 4;
  m[16726215] = 5;
  m[16734885] = 6;
  m[16728765] = 7;
  m[16730805] = 8;
  m[16732845] = 9;
  return m;
}


void updateLEDs(Adafruit_TLC5947 tlc, LedWorker rgb1o, LedWorker rgb1u,
                LedWorker rgb2o, LedWorker rgb2u, LedWorker rgb3o, LedWorker rgb3u, LedWorker rgb4o, LedWorker rgb4u){

    RGB rgb1oColor = rgb1o.getColorForUpdate();
    tlc.setLED(rgb1o.getId(), rgb1oColor.red(), rgb1oColor.green(), rgb1oColor.blue());
    RGB rgb1uColor = rgb1u.getColorForUpdate();
    tlc.setLED(rgb1u.getId(), rgb1uColor.red(), rgb1uColor.green(), rgb1uColor.blue());

    RGB rgb2oColor = rgb2o.getColorForUpdate();
    tlc.setLED(rgb2o.getId(), rgb2oColor.red(), rgb2oColor.green(), rgb2oColor.blue());
    RGB rgb2uColor = rgb2u.getColorForUpdate();
    tlc.setLED(rgb2u.getId(), rgb2uColor.red(), rgb2uColor.green(), rgb2uColor.blue());

    RGB rgb3oColor = rgb3o.getColorForUpdate();
    tlc.setLED(rgb3o.getId(), rgb3oColor.red(), rgb3oColor.green(), rgb3oColor.blue());
    RGB rgb3uColor = rgb3u.getColorForUpdate();
    tlc.setLED(rgb3u.getId(), rgb3uColor.red(), rgb3uColor.green(), rgb3uColor.blue());

    RGB rgb4oColor = rgb4o.getColorForUpdate();
    tlc.setLED(rgb4o.getId(), rgb4oColor.red(), rgb4oColor.green(), rgb4oColor.blue());
    RGB rgb4uColor = rgb4u.getColorForUpdate();
    tlc.setLED(rgb4u.getId(), rgb4uColor.red(), rgb4uColor.green(), rgb4uColor.blue());

    tlc.write();

}
