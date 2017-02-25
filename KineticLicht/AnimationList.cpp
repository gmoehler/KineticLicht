#include "AnimationList.h"

bool Compare_Rows(const unsigned a[8], const unsigned b[8])
{
  return a[0] < b[0];
}

void AnimationList::load(){

unsigned led_test1[][8] = {
    {LED1BOT, 0, 0, BLACK, 0, 2},
    {LED1TOP, 0, 0, BLACK, 0, 2},
    {LED2BOT, 0, 0, BLACK, 0, 2},
    {LED2TOP, 0, 0, BLACK, 0, 2},
    {LED3BOT, 0, 0, BLACK, 0, 2},
    {LED3TOP, 0, 0, BLACK, 0, 2},
    {LED4BOT, 0, 0, BLACK, 0, 2},
    {LED4TOP, 0, 0, BLACK, 0, 2},

    {LED1BOT, 20, 0, RED, 100, 2},
    {LED1TOP, 20, 0, BLACK, 0, 2},

    {LED1BOT, 40, 0, BLACK, 0, 2},
    {LED1TOP, 40, 0, RED, 100, 2},
    {LED2BOT, 40, 0, BLACK, 0, 2},

    {LED1TOP, 60, 0, BLACK, 0, 2},
    {LED2BOT, 60, 0, RED, 100, 2},
    {LED2TOP, 60, 0, BLACK, 0, 2},

    {LED2BOT, 80, 0, BLACK, 0, 2},
    {LED2TOP, 80, 0, RED, 100, 2},
    {LED3BOT, 80, 0, BLACK, 0, 2},

    {LED2TOP, 100, 0, BLACK, 0, 2},
    {LED3BOT, 100, 0, RED, 100, 2},
    {LED3TOP, 100, 0, BLACK, 0, 2},

    {LED3BOT, 120, 0, BLACK, 0, 2},
    {LED3TOP, 120, 0, RED, 100, 2},
    {LED4BOT, 120, 0, BLACK, 0, 2},

    {LED3TOP, 140, 0, BLACK, 0, 2},
    {LED4BOT, 140, 0, RED, 100, 2},
    {LED4TOP, 140, 0, BLACK, 0, 2},

    {LED4BOT, 160, 0, BLACK, 0, 2},
    {LED4TOP, 160, 0, RED, 100, 2},

    {LED4TOP, 180, 0, BLACK, 0, 2}
  };

  _addAsAnimationUint(led_test1, 0);
  /*
  unsigned rows = sizeof(led_test1) / sizeof(led_test1[0]);
  auto led_test1_heap = new unsigned[rows][8]();
  std::copy(&led_test1[0][0], &led_test1[0][0]+rows*8,&led_test1_heap[0][0]);
  _allAnimations[0] = led_test1_heap;

  for (unsigned i=0; i<31; i++){
    for (int j=0; j<8; j++){
      printf("%d ", _allAnimations[0][i][j]);
    }
    printf("\n");
  }
*/
  // LED test: all LEDs change colors at the same time
unsigned led_test2[][8] ={
    {LED1BOT, 3000, RED, 100},
    {LED1TOP, 3000, RED, 100},
    {LED2BOT, 3000, RED, 100},
    {LED2TOP, 3000, RED, 100},
    {LED3BOT, 3000, RED, 100},
    {LED3TOP, 3000, RED, 100},
    {LED4BOT, 3000, RED, 100},
    {LED4TOP, 3000, RED, 100},

    {LED1BOT, 6000, GREEN, 100},
    {LED1TOP, 6000, GREEN, 100},
    {LED2BOT, 6000, GREEN, 100},
    {LED2TOP, 6000, GREEN, 100},
    {LED3BOT, 6000, GREEN, 100},
    {LED3TOP, 6000, GREEN, 100},
    {LED4BOT, 6000, GREEN, 100},
    {LED4TOP, 6000, GREEN, 100},

    {LED1BOT, 9000, BLUE, 100},
    {LED1TOP, 9000, BLUE, 100},
    {LED2BOT, 9000, BLUE, 100},
    {LED2TOP, 9000, BLUE, 100},
    {LED3BOT, 9000, BLUE, 100},
    {LED3TOP, 9000, BLUE, 100},
    {LED4BOT, 9000, BLUE, 100},
    {LED4TOP, 9000, BLUE, 100},

    {LED1BOT, 12000, RED, 0},
    {LED1TOP, 12000, RED, 0},
    {LED2BOT, 12000, RED, 0},
    {LED2TOP, 12000, RED, 0},
    {LED3BOT, 12000, RED, 0},
    {LED3TOP, 12000, RED, 0},
    {LED4BOT, 12000, RED, 0},
    {LED4TOP, 12000, RED, 0},

    {LED1BOT, 12000, RED, 0},
    {LED1TOP, 12000, RED, 0},
    {LED2BOT, 12000, RED, 0},
    {LED2TOP, 12000, RED, 0},
    {LED3BOT, 12000, RED, 0},
    {LED3TOP, 12000, RED, 0},
    {LED4BOT, 12000, RED, 0},
    {LED4TOP, 12000, RED, 0},

    {LED1BOT, 12000, RED, 0},
    {LED1TOP, 12000, RED, 0},
    {LED2BOT, 12000, RED, 0},
    {LED2TOP, 12000, RED, 0},
    {LED3BOT, 12000, RED, 0},
    {LED3TOP, 12000, RED, 0},
    {LED4BOT, 12000, RED, 0},
    {LED4TOP, 12000, RED, 0},

    {LED1BOT, 15000, LILA, 0},
    {LED1TOP, 15000, LILA, 0},
    {LED2BOT, 15000, LILA, 0},
    {LED2TOP, 15000, LILA, 0},
    {LED3BOT, 15000, LILA, 0},
    {LED3TOP, 15000, LILA, 0},
    {LED4BOT, 15000, LILA, 0},
    {LED4TOP, 15000, LILA, 0}

  };

  _addAsAnimationUint(led_test2, 1);

/*
  Animation led_test2;
  led_test2.addKeyFrames(kfs1);
*/
  //_animationList.push_back(led_test2);

/*
      Animation a;
      KeyFrame kf0(LED1BOT, 2000,  GREEN, 100);
      KeyFrame kf1(LED3BOT, 2000,  GREEN, 100);
      std::vector<KeyFrame> kfs = {
        {LED1BOT, 2000,  GREEN, 100},
        {LED1BOT, 4000,  BLUE, 100}
      };
      kfs.push_back(kf0);
      kfs.push_back(kf1);
      a.addKeyFrames(kfs);

  //    _animationList.push_back(a);



//  _animationList.push_back(led_test2);
/*
  std::vector<KeyFrame> kfs3 ={
    {STEPPER1, 0, 0},
    {STEPPER1, 2000, 0},
    {STEPPER1, 9000, 2600},
    {STEPPER1, 12000, 2600},
    {STEPPER1, 26000, 10},

    {STEPPER2,0, 0},
    {STEPPER2,3000, 0},
    {STEPPER2,10000, 2800},
    {STEPPER2,13000, 2800},
    {STEPPER2,27000, 10},

    {LED1TOP, 0,    YELLOW, 0},
    {LED1TOP, 1500, YELLOW, 50},
    {LED1TOP, 2000, YELLOW, 50},
    {LED1TOP, 9000, RED, 100},
    {LED1TOP, 9500, RED, 100},
    {LED1TOP, 11000, BLUE, 100},
    {LED1TOP, 12000, BLUE, 100},
    {LED1TOP, 26000, GREEN, 50},
    {LED1TOP, 27000, GREEN, 50},
    {LED1TOP, 28000, YELLOW, 50},
    {LED1TOP, 33000, YELLOW, 0},

    {LED1BOT, 0,    YELLOW, 0},
    {LED1BOT, 3500, YELLOW, 50},
    {LED1BOT, 4000, YELLOW, 50},
    {LED1BOT, 11000, RED, 100},
    {LED1BOT, 11500, RED, 100},
    {LED1BOT, 13000, BLUE, 100},
    {LED1BOT, 14000, BLUE, 100},
    {LED1BOT, 28000, GREEN, 50},
    {LED1BOT, 29000, GREEN, 50},
    {LED1BOT, 30000, YELLOW, 50},
    {LED1BOT, 35000, YELLOW, 0},

    {LED4TOP, 0,    YELLOW, 0},
    {LED4TOP, 1000, YELLOW, 0},
    {LED4TOP, 2500, YELLOW, 50},
    {LED4TOP, 3000, YELLOW, 50},
    {LED4TOP, 11000, RED, 100},
    {LED4TOP, 11500, RED, 100},
    {LED4TOP, 13000, BLUE, 100},
    {LED4TOP, 14000, BLUE, 100},
    {LED4TOP, 28000, GREEN, 50},
    {LED4TOP, 29000, GREEN, 50},
    {LED4TOP, 30000, YELLOW, 50},
    {LED4TOP, 35000, YELLOW, 50},

    {LED4BOT, 0,    YELLOW, 0},
    {LED4BOT, 1000,    YELLOW, 0},
    {LED4BOT, 4500, YELLOW, 50},
    {LED4BOT, 5000, YELLOW, 50},
    {LED4BOT, 14000, RED, 100},
    {LED4BOT, 13500, RED, 100},
    {LED4BOT, 15000, BLUE, 100},
    {LED4BOT, 16000, BLUE, 100},
    {LED4BOT, 30000, GREEN, 50},
    {LED4BOT, 31000, GREEN, 50},
    {LED4BOT, 32000, YELLOW, 50},
    {LED4BOT, 37000, YELLOW, 0}
  };
//);
//  _animationList.push_back(a1);

  //animation 2
  std::vector<KeyFrame> kfs4 ={
      {STEPPER1, 0, 0},
      {STEPPER4, 0, 0},
      {STEPPER4, 2000, 0},
      {STEPPER1, 4000, 3000},
      {STEPPER4, 6000, 3000},
      {STEPPER1, 8000, 0},
      {STEPPER4, 10000, 0},

      {LED1TOP, 400,  WHITE, 100},
      {LED1TOP, 1000, BLACK, 0},
      {LED1TOP, 2300, BLACK, 0},
      {LED1TOP, 2400, WHITE, 100},
      {LED1TOP, 3000, BLACK, 0},
      {LED1TOP, 20000, WHITE, 100},
      {LED1TOP, 22000, BLACK, 0},

      {LED1BOT, 200,  WHITE, 100},
      {LED1BOT, 800, BLACK, 0},
      {LED1BOT, 2000,  BLACK, 0},
      {LED1BOT, 2200,  WHITE, 100},
      {LED1BOT, 2800, BLACK, 0},
      {LED1BOT, 20000, WHITE, 100},
      {LED1BOT, 22000, BLACK, 0},

      {LED4TOP, 500,  WHITE, 100},
      {LED4TOP, 1000, BLACK, 0},
      {LED4TOP, 3600,  BLACK, 0},
      {LED4TOP, 3700,  WHITE, 100},
      {LED4TOP, 4100, BLACK, 0},
      {LED4TOP, 20000, WHITE, 100},
      {LED4TOP, 22000, BLACK, 0},

      {LED4BOT, 300,  WHITE, 100},
      {LED4BOT, 800, BLACK, 0},
      {LED4BOT, 3500,  BLACK, 0},
      {LED4BOT, 3600,  WHITE, 100},
      {LED4BOT, 4200, BLACK, 0},
      {LED4BOT, 20000, WHITE, 100},
      {LED4BOT, 22000, BLACK, 0}
    };

    _animationList.push_back(a2);
*/
}
