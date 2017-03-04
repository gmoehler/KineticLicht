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

std::map<long, int> create_NumberButtonMap();
int freeRam ();

/*******************
 Create LED objects
********************/

#define LED_DATA   4
#define LED_CLOCK  5
#define LED_LATCH  6
// these are hard-wired in the lib using fast data write
Adafruit_TLC5947 tlc = Adafruit_TLC5947(1, LED_CLOCK, LED_DATA, LED_LATCH);

int fr0 = freeRam();

LedWorker rgb1o = LedWorker (LED1TOP, 6);
LedWorker rgb1u = LedWorker (LED1BOT, 7);
LedWorker rgb2o = LedWorker (LED2TOP, 4);
LedWorker rgb2u = LedWorker (LED2BOT, 5);
LedWorker rgb3o = LedWorker (LED3TOP, 1);
LedWorker rgb3u = LedWorker (LED3BOT, 0);
LedWorker rgb4o = LedWorker (LED4TOP, 3);
LedWorker rgb4u = LedWorker (LED4BOT, 2);

int fr1 = freeRam();

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

AnimationOps aniop(tlc, true);

int fr2 = freeRam();

void setup()
{
  Serial.begin(9600);
  printf("0### FREE RAM: %d\n",  fr0);
  printf("0### FREE RAM: %d\n",  fr1);
  printf("0### FREE RAM: %d\n",  fr2);
  printf("1### FREE RAM: %d\n",  freeRam());
  //Serial.println("start");
  printf("Hello World\n");
  cout << "Start." << endl;

  //AFMS_a.begin();
  //AFMS_b.begin();
  // Change the i2c clock to 400KHz
  TWBR = ((F_CPU / 400000l) - 16) / 2;

  printf("2### FREE RAM: %d\n",  freeRam ());

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
  printf("5### FREE RAM: %d\n",  freeRam ());
  delay(10000);

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

int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
