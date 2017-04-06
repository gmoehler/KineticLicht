#include <Arduino.h>
#include <ArduinoSTL.h>

#include <Adafruit_MotorShield_quick.h>
#include <AccelStepper.h>

#include <Adafruit_TLC5947_fast.h>
#include <Flash.h>

#include "KineticLicht.h"
#include "RGB.h"
#include "StepperWorkerFSM.h"
#include "LedWorker.h"
#include "Animation.h"
#include "AnimationOps.h"

//#define USE_QUICKSTEP

std::map<long, int> create_NumberButtonMap();
int freeRam ();
char pbuffer[PBUFFER_SIZE];

/*******************
 Create LED objects
********************/

#define LED_DATA   4
#define LED_CLOCK  5
#define LED_LATCH  6
// these are hard-wired in the lib using fast data write
Adafruit_TLC5947 tlc(1, LED_CLOCK, LED_DATA, LED_LATCH);

int fr0 = freeRam();

LedWorker rgb1o(LED1TOP, 6);
LedWorker rgb1u(LED1BOT, 7);
LedWorker rgb2o(LED2TOP, 4);
LedWorker rgb2u(LED2BOT, 5);
LedWorker rgb3o(LED3TOP, 1);
LedWorker rgb3u(LED3BOT, 0);
LedWorker rgb4o(LED4TOP, 3);
LedWorker rgb4u(LED4BOT, 2);

int fr1 = freeRam();

/*************************
  Create Stepper objects
*************************/
// two stepper motors one on each port
Adafruit_MotorShield AFMS_a = Adafruit_MotorShield(0x60);
Adafruit_MotorShield AFMS_b = Adafruit_MotorShield(0x61);

Adafruit_StepperMotor *steppermotor1 = AFMS_b.getStepper(200, 2);
Adafruit_StepperMotor *steppermotor2 = AFMS_a.getStepper(200, 2);
Adafruit_StepperMotor *steppermotor3 = AFMS_b.getStepper(200, 1);
Adafruit_StepperMotor *steppermotor4 = AFMS_a.getStepper(200, 1);

int fr2 = freeRam();

// you can change these to SINGLE, DOUBLE or INTERLEAVE or MICROSTEP!
uint8_t style = DOUBLE;
void forwardstep1() {
#ifndef USE_QUICKSTEP
  steppermotor1->onestep(FORWARD, style);
#else
  steppermotor1->quickstep(FORWARD);
#endif
}
void backwardstep1() {
  #ifndef USE_QUICKSTEP
    steppermotor1->onestep(BACKWARD, style);
  #else
    steppermotor1->quickstep(BACKWARD);
  #endif
}
AccelStepper astepper1(forwardstep1, backwardstep1);

void forwardstep2() {
  #ifndef USE_QUICKSTEP
    steppermotor2->onestep(FORWARD, style);
  #else
    steppermotor2->quickstep(FORWARD);
  #endif
}
void backwardstep2() {
  #ifndef USE_QUICKSTEP
    steppermotor2->onestep(BACKWARD, style);
  #else
    steppermotor2->quickstep(BACKWARD);
  #endif
}
AccelStepper astepper2(forwardstep2, backwardstep2);

void forwardstep3() {
  #ifndef USE_QUICKSTEP
    steppermotor3->onestep(FORWARD, style);
  #else
    steppermotor3->quickstep(FORWARD);
  #endif
}
void backwardstep3() {
  #ifndef USE_QUICKSTEP
    steppermotor3->onestep(BACKWARD, style);
  #else
    steppermotor3->quickstep(BACKWARD);
  #endif
}
AccelStepper astepper3(forwardstep3, backwardstep3);

void forwardstep4() {
  #ifndef USE_QUICKSTEP
    steppermotor4->onestep(FORWARD, style);
  #else
    steppermotor4->quickstep(FORWARD);
  #endif
}
void backwardstep4() {
  #ifndef USE_QUICKSTEP
    steppermotor4->onestep(BACKWARD, style);
  #else
    steppermotor4->quickstep(BACKWARD);
  #endif
}
AccelStepper astepper4(forwardstep4, backwardstep4);

int fr3 = freeRam();

StepperWorkerFSM  *sworker1 = new StepperWorkerFSM(STEPPER1, astepper1, 53, true);
StepperWorkerFSM  *sworker2 = new StepperWorkerFSM(STEPPER2, astepper2, 49, true); // true seems wrong, but does the right thing...
StepperWorkerFSM  *sworker3 = new StepperWorkerFSM(STEPPER3, astepper3, 51, false);
StepperWorkerFSM  *sworker4 = new StepperWorkerFSM(STEPPER4, astepper4, 47, true);

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

int fr4 = freeRam();
AnimationOps aniop(tlc, true);
int fr5 = freeRam();

void setup()
{
  Serial.begin(9600);
  FPRINTF1(kin_msg0, "0### FREE RAM: %d\n",  fr0);
  FPRINTF1(kin_msg1, "1### FREE RAM: %d\n",  fr1);
  FPRINTF1(kin_msg2, "2### FREE RAM: %d\n",  fr2);
  FPRINTF1(kin_msg3, "3### FREE RAM: %d\n",  fr3);
  FPRINTF1(kin_msg4, "4### FREE RAM: %d\n",  fr4);
  FPRINTF1(kin_msg5, "5### FREE RAM: %d\n",  fr5);
  FPRINTF1(kin_msg6, "6### FREE RAM: %d\n",  freeRam());

  //printf("Hello World\n");
  std::cout << "Start." << std::endl;

  AFMS_a.begin(2000);
  AFMS_b.begin(2000);

  // Change the i2c clock to 400KHz
  const  long freq=800000l;
  TWBR = ((F_CPU / freq) - 16) / 2;

  FPRINTF1(kin_msg7,"7### FREE RAM: %d\n",  freeRam ());

  aniop.addStepperWorker(sworker1);
  aniop.addStepperWorker(sworker2);
  aniop.addStepperWorker(sworker3);
  aniop.addStepperWorker(sworker4);
  aniop.addLedWorker(&rgb1o);
  aniop.addLedWorker(&rgb1u);
  aniop.addLedWorker(&rgb2o);
  aniop.addLedWorker(&rgb2u);
  aniop.addLedWorker(&rgb3o);
  aniop.addLedWorker(&rgb3u);
  aniop.addLedWorker(&rgb4o);
  aniop.addLedWorker(&rgb4u);

  aniop.init(SINGLE_ANIMATION, 0, false);

  FPRINTF1(kin_msg8,"8### FREE RAM: %d\n",  freeRam ());

  //irrecv.enableIRIn(); // Start the IR receiver
}

/************
    Loop
************/

long loopNo = 0;

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
  loopNo++;
  int fr = freeRam();
  if (loopNo % 500 == 0){
//    FPRINTF1(kin_msg9, "******** MEMORY: %d *******\n", fr);
  }

  if (fr > 200){
  //  aniop.loop();
  }
  else {
    FPRINTF1(kin_msg10, "ERROR! Memory exhausted: %d Bytes left\n", fr);
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

int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
