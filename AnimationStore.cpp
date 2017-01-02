#include "AnimationStore.h"

// full height: 4200 = 2100mm

Animation& AnimationStore::getAnimation(int id){
  return animation[id];
}

AnimationStore::AnimationStore()
{
  Animation a1;
  animation.push_back(a1); 
  
  a1.addMotorKeyFrames(1, {
    {0, 0},
    {2000, 0},
    {9000, 2600},
    {12000, 2600},
    {26000, 10}    
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

  a1.addMotorKeyFrames(4, 
  {
    {0, 0},
    {3000, 0},
    {10000, 2800},
    {13000, 2800},
    {27000, 10},
  });


  a1.addRgbKeyFrames(1, 
  {
    {0,    YELLOW, 0},
    {1500, YELLOW, 50},
    {2000, YELLOW, 50},
    {9000, RED, 100}, 
    {9500, RED, 100},
    {11000, BLUE, 100},
    {12000, BLUE, 100},
    {26000, GREEN, 50},
    {27000, GREEN, 50},
    {28000, YELLOW, 50},
    {33000, YELLOW, 0},
  }, TOP);

  a1.addRgbKeyFrames(1, {
    {0,    YELLOW, 0},
    {3500, YELLOW, 50},
    {4000, YELLOW, 50},
    {11000, RED, 100},
    {11500, RED, 100}, 
    {13000, BLUE, 100}, 
    {14000, BLUE, 100},
    {28000, GREEN, 50},
    {29000, GREEN, 50},
    {30000, YELLOW, 50},
    {35000, YELLOW, 0},
  }, BOTTOM);

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

  a1.addRgbKeyFrames(4, {
    {0,    YELLOW, 0},
    {1000, YELLOW, 0},
    {2500, YELLOW, 50},
    {3000, YELLOW, 50},
    {11000, RED, 100}, 
    {11500, RED, 100}, 
    {13000, BLUE, 100}, 
    {14000, BLUE, 100}, 
    {28000, GREEN, 50},
    {29000, GREEN, 50},
    {30000, YELLOW, 50},
    {35000, YELLOW, 50},
  }, TOP);

  a1.addRgbKeyFrames(4, {
    {0,    YELLOW, 0},
    {1000,    YELLOW, 0},
    {4500, YELLOW, 50},
    {5000, YELLOW, 50},
    {14000, RED, 100},
    {13500, RED, 100},
    {15000, BLUE, 100}, 
    {16000, BLUE, 100}, 
    {30000, GREEN, 50},
    {31000, GREEN, 50},
    {32000, YELLOW, 50},
    {37000, YELLOW, 0},
  }, TOP);



//animation 2

  Animation a2;
  animation.push_back(a2);

  a2.addMotorKeyFrames(
  {
    {1, 0, 0},
    {4, 0, 0},
    {4, 2000, 0},
    {1, 4000, 3000},
    {4, 6000, 3000},
    {1, 8000, 0},
    {4, 10000, 0},
  });
  
  a1.addRgbKeyFrames(1, {
  {400,  WHITE, 100},
  {1000, BLACK, 0},
  {2300,  BLACK, 0},
  {2400,  WHITE, 100},
  {3000, BLACK, 0},
  {20000, WHITE, 100},
  {22000, BLACK, 0},
  }, TOP);

  a1.addRgbKeyFrames(1, {
  {200,  WHITE, 100},
  {800, BLACK, 0},
  {2000,  BLACK, 0},
  {2200,  WHITE, 100},
  {2800, BLACK, 0},
  {20000, WHITE, 100},
  {22000, BLACK, 0},
  }, BOTTOM);

  a1.addRgbKeyFrames(4, {
  {500,  WHITE, 100},
  {1000, BLACK, 0},
  {3600,  BLACK, 0},
  {3700,  WHITE, 100},
  {4100, BLACK, 0},
  {20000, WHITE, 100},
  {22000, BLACK, 0},
  }, TOP);
  
  a1.addRgbKeyFrames(4, {
  {300,  WHITE, 100},
  {800, BLACK, 0},
  {3500,  BLACK, 0},
  {3600,  WHITE, 100},
  {4200, BLACK, 0},
  {20000, WHITE, 100},
  {22000, BLACK, 0},
  }, BOTTOM);

}
