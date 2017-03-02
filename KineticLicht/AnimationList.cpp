#include "AnimationList.h"

bool Compare_Rows(const unsigned a[8], const unsigned b[8])
{
  return a[0] < b[0];
}

void AnimationList::load(){

int animationId = 0;

unsigned led_test1[][8] = {
    {LED1BOT, 0, 0, BLACK, 0, 2},
    {LED1TOP, 0, 0, BLACK, 0, 2},
    {LED2BOT, 0, 0, BLACK, 0, 2},
    {LED2TOP, 0, 0, BLACK, 0, 2},
    {LED3BOT, 0, 0, BLACK, 0, 2},
    {LED3TOP, 0, 0, BLACK, 0, 2},
    {LED4BOT, 0, 0, BLACK, 0, 2},
    {LED4TOP, 0, 0, BLACK, 0, 2},

    {LED1BOT, 20, 0, RED, 1, 2},
    {LED1TOP, 20, 0, BLACK, 0, 2},

    {LED1BOT, 40, 0, BLACK, 0, 2},
    {LED1TOP, 40, 0, RED, 1, 2},
    {LED2BOT, 40, 0, BLACK, 0, 2},

    {LED1TOP, 60, 0, BLACK, 0, 2},
    {LED2BOT, 60, 0, RED, 1, 2},
    {LED2TOP, 60, 0, BLACK, 0, 2},

    {LED2BOT, 80, 0, BLACK, 0, 2},
    {LED2TOP, 80, 0, RED, 1, 2},
    {LED3BOT, 80, 0, BLACK, 0, 2},

    {LED2TOP, 1, 0, BLACK, 0, 2},
    {LED3BOT, 1, 0, RED, 1, 2},
    {LED3TOP, 1, 0, BLACK, 0, 2},

    {LED3BOT, 120, 0, BLACK, 0, 2},
    {LED3TOP, 120, 0, RED, 1, 2},
    {LED4BOT, 120, 0, BLACK, 0, 2},

    {LED3TOP, 140, 0, BLACK, 0, 2},
    {LED4BOT, 140, 0, RED, 1, 2},
    {LED4TOP, 140, 0, BLACK, 0, 2},

    {LED4BOT, 160, 0, BLACK, 0, 2},
    {LED4TOP, 160, 0, RED, 1, 2},

    {LED4TOP, 180, 0, BLACK, 0, 2}
  };

  int rows = sizeof(led_test1) / sizeof(led_test1[0]);
  _addAsAnimationUint(led_test1, rows, animationId++);

  // LED test: all LEDs change colors at the same time
unsigned led_test2[][8] ={
    {LED1BOT, 30, RED, 100},
    {LED1TOP, 30, RED, 100},
    {LED2BOT, 30, RED, 100},
    {LED2TOP, 30, RED, 100},
    {LED3BOT, 30, RED, 100},
    {LED3TOP, 30, RED, 100},
    {LED4BOT, 30, RED, 100},
    {LED4TOP, 30, RED, 100},

    {LED1BOT, 60, GREEN, 100},
    {LED1TOP, 60, GREEN, 100},
    {LED2BOT, 60, GREEN, 100},
    {LED2TOP, 60, GREEN, 100},
    {LED3BOT, 60, GREEN, 100},
    {LED3TOP, 60, GREEN, 100},
    {LED4BOT, 60, GREEN, 100},
    {LED4TOP, 60, GREEN, 100},

    {LED1BOT, 90, BLUE, 100},
    {LED1TOP, 90, BLUE, 100},
    {LED2BOT, 90, BLUE, 100},
    {LED2TOP, 90, BLUE, 100},
    {LED3BOT, 90, BLUE, 100},
    {LED3TOP, 90, BLUE, 100},
    {LED4BOT, 90, BLUE, 100},
    {LED4TOP, 90, BLUE, 100},

    {LED1BOT, 120, RED, 0},
    {LED1TOP, 120, RED, 0},
    {LED2BOT, 120, RED, 0},
    {LED2TOP, 120, RED, 0},
    {LED3BOT, 120, RED, 0},
    {LED3TOP, 120, RED, 0},
    {LED4BOT, 120, RED, 0},
    {LED4TOP, 120, RED, 0},

    {LED1BOT, 120, RED, 0},
    {LED1TOP, 120, RED, 0},
    {LED2BOT, 120, RED, 0},
    {LED2TOP, 120, RED, 0},
    {LED3BOT, 120, RED, 0},
    {LED3TOP, 120, RED, 0},
    {LED4BOT, 120, RED, 0},
    {LED4TOP, 120, RED, 0},

    {LED1BOT, 120, RED, 0},
    {LED1TOP, 120, RED, 0},
    {LED2BOT, 120, RED, 0},
    {LED2TOP, 120, RED, 0},
    {LED3BOT, 120, RED, 0},
    {LED3TOP, 120, RED, 0},
    {LED4BOT, 120, RED, 0},
    {LED4TOP, 120, RED, 0},

    {LED1BOT, 150, LILA, 0},
    {LED1TOP, 150, LILA, 0},
    {LED2BOT, 150, LILA, 0},
    {LED2TOP, 150, LILA, 0},
    {LED3BOT, 150, LILA, 0},
    {LED3TOP, 150, LILA, 0},
    {LED4BOT, 150, LILA, 0},
    {LED4TOP, 150, LILA, 0}

  };

  rows = sizeof(led_test2) / sizeof(led_test2[0]);
  _addAsAnimationUint(led_test2, rows, animationId++);

  unsigned a1[][8] = {
    {STEPPER1, 0, 0},
    {STEPPER1, 20, 0},
    {STEPPER1, 90, 2600},
    {STEPPER1, 120, 2600},
    {STEPPER1, 260, 10},

    {STEPPER2,0, 0},
    {STEPPER2,30, 0},
    {STEPPER2,100, 2800},
    {STEPPER2,130, 2800},
    {STEPPER2,270, 10},

    {LED1TOP, 0,    YELLOW, 0},
    {LED1TOP, 15, YELLOW, 50},
    {LED1TOP, 20, YELLOW, 50},
    {LED1TOP, 90, RED, 100},
    {LED1TOP, 95, RED, 100},
    {LED1TOP, 110, BLUE, 100},
    {LED1TOP, 120, BLUE, 100},
    {LED1TOP, 260, GREEN, 50},
    {LED1TOP, 270, GREEN, 50},
    {LED1TOP, 280, YELLOW, 50},
    {LED1TOP, 330, YELLOW, 0},

    {LED1BOT, 0,    YELLOW, 0},
    {LED1BOT, 35, YELLOW, 50},
    {LED1BOT, 40, YELLOW, 50},
    {LED1BOT, 110, RED, 100},
    {LED1BOT, 115, RED, 100},
    {LED1BOT, 130, BLUE, 100},
    {LED1BOT, 140, BLUE, 100},
    {LED1BOT, 280, GREEN, 50},
    {LED1BOT, 290, GREEN, 50},
    {LED1BOT, 300, YELLOW, 50},
    {LED1BOT, 350, YELLOW, 0},

    {LED4TOP, 0,    YELLOW, 0},
    {LED4TOP, 10, YELLOW, 0},
    {LED4TOP, 25, YELLOW, 50},
    {LED4TOP, 30, YELLOW, 50},
    {LED4TOP, 110, RED, 100},
    {LED4TOP, 115, RED, 100},
    {LED4TOP, 130, BLUE, 100},
    {LED4TOP, 140, BLUE, 100},
    {LED4TOP, 280, GREEN, 50},
    {LED4TOP, 290, GREEN, 50},
    {LED4TOP, 300, YELLOW, 50},
    {LED4TOP, 350, YELLOW, 50},

    {LED4BOT, 0,    YELLOW, 0},
    {LED4BOT, 10,    YELLOW, 0},
    {LED4BOT, 45, YELLOW, 50},
    {LED4BOT, 50, YELLOW, 50},
    {LED4BOT, 140, RED, 100},
    {LED4BOT, 135, RED, 100},
    {LED4BOT, 150, BLUE, 100},
    {LED4BOT, 160, BLUE, 100},
    {LED4BOT, 300, GREEN, 50},
    {LED4BOT, 310, GREEN, 50},
    {LED4BOT, 320, YELLOW, 50},
    {LED4BOT, 370, YELLOW, 0}
  };

  rows = sizeof(a1) / sizeof(a1[0]);
  _addAsAnimationUint(a1, rows, animationId++);

  //animation 2
  unsigned a2[][8] = {
      {STEPPER1, 0, 0},
      {STEPPER4, 0, 0},
      {STEPPER4, 20, 0},
      {STEPPER1, 40, 3000},
      {STEPPER4, 60, 3000},
      {STEPPER1, 80, 0},
      {STEPPER4, 100, 0},

      {LED1TOP, 4,  WHITE, 100},
      {LED1TOP, 10, BLACK, 0},
      {LED1TOP, 23, BLACK, 0},
      {LED1TOP, 24, WHITE, 100},
      {LED1TOP, 30, BLACK, 0},
      {LED1TOP, 200, WHITE, 100},
      {LED1TOP, 220, BLACK, 0},

      {LED1BOT, 2,  WHITE, 100},
      {LED1BOT, 8, BLACK, 0},
      {LED1BOT, 20,  BLACK, 0},
      {LED1BOT, 22,  WHITE, 100},
      {LED1BOT, 28, BLACK, 0},
      {LED1BOT, 200, WHITE, 100},
      {LED1BOT, 220, BLACK, 0},

      {LED4TOP, 5,  WHITE, 100},
      {LED4TOP, 10, BLACK, 0},
      {LED4TOP, 36,  BLACK, 0},
      {LED4TOP, 37,  WHITE, 100},
      {LED4TOP, 41, BLACK, 0},
      {LED4TOP, 200, WHITE, 100},
      {LED4TOP, 220, BLACK, 0},

      {LED4BOT, 3,  WHITE, 100},
      {LED4BOT, 8, BLACK, 0},
      {LED4BOT, 35,  BLACK, 0},
      {LED4BOT, 36,  WHITE, 100},
      {LED4BOT, 42, BLACK, 0},
      {LED4BOT, 200, WHITE, 100},
      {LED4BOT, 220, BLACK, 0}
    };

    rows = sizeof(a2) / sizeof(a2[0]);
    _addAsAnimationUint(a2, rows, animationId++);
    
}
