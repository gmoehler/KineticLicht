#include <Arduino.h>
#include <ArduinoSTL.h>

#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

#include "Adafruit_TLC5947.h"

#include "RGB.h"
#include "StepperWorker.h"
#include "LedWorker.h"
#include "Animation.h"
#include "AnimationOps.h"

std::vector<Animation> loadAnimations();
std::map<long, int> create_NumberButtonMap();

/*************************
create LED objects
*************************/

#define LED_DATA   4
#define LED_CLOCK  5
#define LED_LATCH  6
// these are hard-wired in the lib using fast data write
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, LED_CLOCK, LED_DATA, LED_LATCH);

LedWorker rgb1o = LedWorker (LED1TOP, 6);
LedWorker rgb1u = LedWorker (LED1BOT, 7);
LedWorker rgb2o = LedWorker (LED2TOP, 4);
LedWorker rgb2u = LedWorker (LED2BOT, 5);
LedWorker rgb3o = LedWorker (LED3TOP, 1);
LedWorker rgb3u = LedWorker (LED3BOT, 0);
LedWorker rgb4o = LedWorker (LED4TOP, 3);
LedWorker rgb4u = LedWorker (LED4BOT, 2);

/*************************
create Stepper objects
*************************/
// two stepper motors one on each port
/*Adafruit_MotorShield AFMS_a = Adafruit_MotorShield(0x60);
Adafruit_MotorShield AFMS_b = Adafruit_MotorShield(0x61);

Adafruit_StepperMotor *steppermotor1 = AFMS_b.getStepper(200, 2);
Adafruit_StepperMotor *steppermotor2 = AFMS_a.getStepper(200, 2);
Adafruit_StepperMotor *steppermotor3 = AFMS_a.getStepper(200, 1);
Adafruit_StepperMotor *steppermotor4 = AFMS_b.getStepper(200, 1);

// you can change these to DOUBLE or INTERLEAVE or MICROSTEP!
void forwardstep1() {
steppermotor1->onestep(FORWARD, INTERLEAVE);
}
void backwardstep1() {
steppermotor1->onestep(BACKWARD, INTERLEAVE);
}
AccelStepper astepper1(forwardstep1, backwardstep1);

void forwardstep2() {
steppermotor2->onestep(FORWARD, INTERLEAVE);
}
void backwardstep2() {
steppermotor2->onestep(BACKWARD, INTERLEAVE);
}
AccelStepper astepper2(forwardstep2, backwardstep2);

void forwardstep3() {
steppermotor3->onestep(FORWARD, INTERLEAVE);
}
void backwardstep3() {
steppermotor3->onestep(BACKWARD, INTERLEAVE);
}
AccelStepper astepper3(forwardstep3, backwardstep3);

void forwardstep4() {
steppermotor4->onestep(FORWARD, INTERLEAVE);
}
void backwardstep4() {
steppermotor4->onestep(BACKWARD, INTERLEAVE);
}
AccelStepper astepper4(forwardstep4, backwardstep4);
*/
//StepperWorker  sworker1 = StepperWorker(STEPPER1, astepper1, 53, true);
//StepperWorker  sworker2 = StepperWorker(STEPPER2, astepper2, 49, false);
//StepperWorker  sworker3 = StepperWorker(STEPPER3, astepper3, 51, false);
//StepperWorker  sworker4 = StepperWorker(STEPPER4, astepper4, 47, true);

/****************************
Create IR remote objects
****************************/

/*int IR_RECV_PIN = 11;

IRrecv irrecv(IR_RECV_PIN);
decode_results irResults;
std::map<long, int> numberButtons = create_NumberButtonMap();
*/

/************
Setup
************/

AnimationOps aniop(tlc);

void setup()
{
  Serial.begin(9600);
  //Serial.println("start");
  printf("Hello World\n");
  cout << "Start." << endl;

  //AFMS_a.begin();
  //AFMS_b.begin();
  // Change the i2c clock to 400KHz
  TWBR = ((F_CPU / 400000l) - 16) / 2;

  /*std::vector<Animation> animations = loadAnimations();
  for (unsigned i=0; i< animations.size(); i++){
  aniop.addAnimation(animations[i]);}*/

  std::vector<Animation> animations;

  Animation led_test1({
    {LED1BOT, 0,  BLACK, 0},
    {LED1TOP, 0,  BLACK, 0},
    {LED2BOT, 0,  BLACK, 0},
    {LED2TOP, 0,  BLACK, 0},
    {LED3BOT, 0,  BLACK, 0},
    {LED3TOP, 0,  BLACK, 0},
    {LED4BOT, 0,  BLACK, 0},
    {LED4TOP, 0,  BLACK, 0},

    {LED1BOT, 2000,  RED, 100},
    {LED1TOP, 2000,  BLACK, 0},

    {LED1BOT, 4000, BLACK, 0},
    {LED1TOP, 4000, RED, 100},
    {LED2BOT, 4000, BLACK, 0},

    {LED1TOP, 6000, BLACK, 0},
    {LED2BOT, 6000, RED, 100},
    {LED2TOP, 6000, BLACK, 0},

    {LED2BOT, 8000, BLACK, 0},
    {LED2TOP, 8000, RED, 100},
    {LED3BOT, 8000, BLACK, 0},

    {LED2TOP, 10000, BLACK, 0},
    {LED3BOT, 10000, RED, 100},
    {LED3TOP, 10000, BLACK, 0},

    {LED3BOT, 12000, BLACK, 0},
    {LED3TOP, 12000, RED, 100},
    {LED4BOT, 12000, BLACK, 0},

    {LED3TOP, 14000, BLACK, 0},
    {LED4BOT, 14000, RED, 100},
    {LED4TOP, 14000, BLACK, 0},

    {LED4BOT, 16000, BLACK, 0},
    {LED4TOP, 16000, RED, 100},

    {LED4TOP, 18000, BLACK, 0}
  });

  //animations.push_back(led_test1);
  aniop.addAnimation(led_test1);

  /*for (unsigned i=0; i< led_test1.numberOfKeyFrames(); i++){
  led_test1.getKeyFrame(i).printKeyFrame();}*/

  //aniop.addStepperWorker(&sworker1);
  //aniop.addStepperWorker(&sworker2);
  //aniop.addStepperWorker(&sworker3);
  //aniop.addStepperWorker(&sworker4);
  aniop.addLedWorker(&rgb1o);
  aniop.addLedWorker(&rgb1u);
  aniop.addLedWorker(&rgb2o);
  aniop.addLedWorker(&rgb2u);
  aniop.addLedWorker(&rgb3o);
  aniop.addLedWorker(&rgb3u);
  aniop.addLedWorker(&rgb4o);
  aniop.addLedWorker(&rgb4u);

  aniop.init(SINGLE_ANIMATION, 0, false);

  //irrecv.enableIRIn(); // Start the IR receiver
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
  if (numberButtons.count(num) == 1 && num < anops.getNumAnimations()) {
  animation = anops.getAnimation(num);
  delay(1000);
  state = ANIMATION_CALIBRATING;
}
irrecv.resume(); // Receive the next value
}
*/

aniop.loop();
delay(500);

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
