#include "AnimationStore.h"

// full height: 4200 = 2100mm

Animation& AnimationStore::getAnimation(int id){
  return animation[id];
}

AnimationStore::AnimationStore()
{
  Animation a1;
  animation.push_back(a1); 
  
  a1.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER1, 2000, 0},
    {STEPPER1, 9000, 2600},
    {STEPPER1, 12000, 2600},
    {STEPPER1, 26000, 10}
  });

  /*
    a1.addMotorKeyFrames(2, {
    {5000, 1000},
    {7000, 200}
    });

    a1.addMotorKeyFrames(3, {
    {5000, 2000},
    {7000, 1500}
    });
  */

  a1.addKeyFrames({
    {STEPPER2,0, 0},
    {STEPPER2,3000, 0},
    {STEPPER2,10000, 2800},
    {STEPPER2,13000, 2800},
    {STEPPER2,27000, 10},
  });


  a1.addKeyFrames({
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
  });

  a1.addKeyFrames({
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
  });

  /*
    a1.addRgbKeyFrames(2, {
    {2000, RED},
    {4000, YELLOW},
    {7000, BLUE}
    }, TOP);
    a1.addRgbKeyFrames(2, {
    {2000, RED},
    {4000, YELLOW},
    {7000, BLUE}
    }, BOTTOM);
    a1.addRgbKeyFrames(3, {
    {2000, RED},
    {4000, YELLOW},
    {7000, BLUE}
    }, TOP);
    a1.addRgbKeyFrames(3, {
    {2000, RED},
    {4000, YELLOW},
    {7000, BLUE}
    }, BOTTOM);
  */

  a1.addKeyFrames({
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
  });

  a1.addKeyFrames({
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
    {LED4BOT, 37000, YELLOW, 0},
  });



//animation 2

  Animation a2;
  animation.push_back(a2);

  a2.addKeyFrames(
  {
    {STEPPER1, 0, 0},
    {STEPPER4, 0, 0},
    {STEPPER4, 2000, 0},
    {STEPPER1, 4000, 3000},
    {STEPPER4, 6000, 3000},
    {STEPPER1, 8000, 0},
    {STEPPER4, 10000, 0},
  });
  
  a2.addKeyFrames({
  {LED1TOP, 400,  WHITE, 100},
  {LED1TOP, 1000, BLACK, 0},
  {LED1TOP, 2300,  BLACK, 0},
  {LED1TOP, 2400,  WHITE, 100},
  {LED1TOP, 3000, BLACK, 0},
  {LED1TOP, 20000, WHITE, 100},
  {LED1TOP, 22000, BLACK, 0},
  });

  a2.addKeyFrames({
  {LED1BOT, 200,  WHITE, 100},
  {LED1BOT, 800, BLACK, 0},
  {LED1BOT, 2000,  BLACK, 0},
  {LED1BOT, 2200,  WHITE, 100},
  {LED1BOT, 2800, BLACK, 0},
  {LED1BOT, 20000, WHITE, 100},
  {LED1BOT, 22000, BLACK, 0},
  });

  a2.addKeyFrames({
  {LED4TOP, 500,  WHITE, 100},
  {LED4TOP, 1000, BLACK, 0},
  {LED4TOP, 3600,  BLACK, 0},
  {LED4TOP, 3700,  WHITE, 100},
  {LED4TOP, 4100, BLACK, 0},
  {LED4TOP, 20000, WHITE, 100},
  {LED4TOP, 22000, BLACK, 0},
  });
  
  a2.addKeyFrames({
  {LED4BOT, 300,  WHITE, 100},
  {LED4BOT, 800, BLACK, 0},
  {LED4BOT, 3500,  BLACK, 0},
  {LED4BOT, 3600,  WHITE, 100},
  {LED4BOT, 4200, BLACK, 0},
  {LED4BOT, 20000, WHITE, 100},
  {LED4BOT, 22000, BLACK, 0},
  });

}
