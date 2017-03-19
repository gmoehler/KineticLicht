#include "test.h"

extern void expectAnimation(KeyFrame kf, int id, long time, long pos, int red, int green, int blue);
/*
typedef unsigned int animation_as_uint_t [7];

animation_as_uint_t* allV[4];
int rows = 0;

animation_as_uint_t* getAnis(int id){
  unsigned v[][7] = {
    {LED1BOT, 20, 0, RED, 100},
    {LED1TOP, 20, 0, BLACK, 0}
  };

  rows = sizeof(v) / sizeof(v[0]);
  printf("sizes v: %d - %d\n",  sizeof(v),  sizeof(v[0]));
  auto v_heap = new unsigned[rows][7];
  printf("sizes v_heap: %d - %d\n",  sizeof(v_heap),  sizeof(v_heap[0]));
  std::copy(&v[0][0], &v[0][0]+rows*7,&v_heap[0][0]);
  //int rows1 = sizeof(v_heap) / sizeof(v_heap[0]);
  printf("sizes v_heap: %d - %d\n",  sizeof(v_heap),  sizeof(v_heap[0]));

  //TODO: check why this fails
  //EXPECT_EQ(rows, rows1);

  for (int i=0; i<rows; i++){
    for (int j=0; j<7; j++){
      printf("%u ", v[i][j]);
    }
    printf("\n");
  }
  for (int i=0; i<rows; i++){
    for (int j=0; j<7; j++){
      printf("%u ", v_heap[i][j]);
    }
    printf("\n");
  }

  allV[0] = v_heap;

  for (int i=0; i<rows; i++){
    for (int j=0; j<7; j++){
      printf("%u ", allV[0][i][j]);
    }
    printf("\n");
  }

  return allV[id];
}

void cleanupAnis(){
  for (int i=0; i<4; i++){
    delete[] allV[i];
  }
}

TEST(AnimationOps_tests, array_stuff){

  animation_as_uint_t* as = getAnis(0);

  Adafruit_TLC5947 tlc = Adafruit_TLC5947();
  AnimationOps ao(tlc, true);

  int num0 = ao.getNumAnimations();
  EXPECT_EQ(4, num0);

  for (int i=0; i<rows; i++){
    for (int j=0; j<7; j++){
      printf("%u ",as[i][j]);
    }
    printf("\n");
  }

  cleanupAnis();

  printf ("current animation...\n");
  ao.selectAnimation(1);
  Animation& an0 = ao._getCurrentAnimation();
  EXPECT_EQ(6, an0.numberOfKeyFrames());
  EXPECT_FALSE(an0.containsMotorFrames());
  //an0.printAnimation();

}
*/

void addAnimation(int ***ary, int t [][5], int rows, int i){

  ary[i] = new int*[rows];
  for(int j= 0; j < rows; ++j) {
    ary[i][j] = new int[5];
    for(int k=0; k< 5; ++k) {
      ary[i][j][k] = t[j][k];
    }
  }
}

TEST(AnimationOps, animationArray){

  int t1[][5] = {
    {0, 0, 0, 0, 0},
  };

  int t2[][5] = {
    {0, 0, 0, 0, 0},
    {1, 1, 1 ,1 , 1},
  };

  int t3[][5] = {
    {0, 0, 0, 0, 0},
    {1, 1, 1 ,1, 1},
    {2, 2, 2, 2, 2},
  };

  int rows[3] = {1,2,3};

  int ***ary = new int**[3];
  addAnimation(ary, t1,rows[0], 0);
  addAnimation(ary, t2,rows[1], 1);
  addAnimation(ary, t3,rows[2], 2);

/*
  int ***ary = new int**[sizeZ];
  for(int i= 0;  i< sizeZ; ++i) {
    ary[i] = new int*[rows[i]];
    for(int j= 0; j < rows[i]; ++j) {
      ary[i][j] = new int[sizeX];
      for(int k=0; k< sizeX; ++k) {
        ary[i][j][k] = 99;
        switch (i){
          case 0:
          ary[i][j][k] = t1[j][k];
          break;
          case 1:
          ary[i][j][k] = t2[j][k];
          break;
          case 2:
          ary[i][j][k] = t3[j][k];
          break;
        }
      }
    }
  }
*/

  printf("hallo\n");
  for (int i=0; i<3; i++){
    for (int j=0; j<rows[i]; j++){
      for (int k=0; k<5; k++){
        printf("%d", ary[i][j][k]);
      }
      printf ("\n");
      delete[] ary[i][j];
    }
    delete[] ary[i];
  }

  delete[] ary;
}

TEST(AnimationOps, animationList){

  AnimationList al(true);

  unsigned **ani = al.getAnimationAsUint(0);

  for (int i=0; i<4; i++){
    for (int j=0; j<7; j++){
      printf("%u ",ani[i][j]);
    }
    printf("\n");
    KeyFrame kf(ani[i]);
    kf.printKeyFrame();
  }

  //animation_as_uint_t *a;
  //a = new animation_as_uint_t[4];

  printf("Done.\n");
  int numKf = al.getNumKeyFrames(0);
  Animation animation(ani, numKf);
  EXPECT_EQ( numKf, animation.numberOfKeyFrames());
  animation.printAnimation();

}

TEST(AnimationOps_tests, scenario_single){

  Adafruit_TLC5947 tlc = Adafruit_TLC5947();
  AnimationOps ao(tlc, true);
  EXPECT_EQ(3, ao.getNumAnimations());

  AccelStepper acs = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (STEPPER1, acs, pin, false);

  LedWorker lw = LedWorker (LED1TOP, 0);

  ao.addStepperWorker(&sw);
  ao.addLedWorker(&lw);

  ao.init(SINGLE_ANIMATION, 0, false);
  EXPECT_EQ(ao.getState(), ANIMATION_INIT);
  /*
  {LED1TOP, 0, 0, BLACK, 0},
  {LED1TOP, 20, 0, BLACK, 0},
  {LED1TOP, 40, 0, RED, 100},
  {LED4TOP, 180, 0, BLACK, 0}
*/

  for (int i=0;i<22;i++){
    ao.loop();
    printf("%d +++++++%d++++++++\n",i,ao.getState());

    if (i>18){
      EXPECT_EQ(ANIMATION_FINISHED, ao.getState());
    }
    else{
      EXPECT_EQ(ANIMATION_ACTIVE, ao.getState());
    }
  }
}

TEST(AnimationOps_tests, scenario_loop){

  Adafruit_TLC5947 tlc = Adafruit_TLC5947();
  AnimationOps ao(tlc, true);
  EXPECT_EQ(3, ao.getNumAnimations());

  AccelStepper acs = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (STEPPER1, acs, pin, false);

  LedWorker lw = LedWorker (LED1TOP, 0);

  ao.addStepperWorker(&sw);
  ao.addLedWorker(&lw);

  ao.init(LOOP_ANIMATION, 0, false);
  EXPECT_EQ(ao.getState(), ANIMATION_INIT);

  // will run tru animations 0 and 1 and start with 2
  for (int i=0;i<45;i++){
    ao.loop();
    printf("%d +++++++%d++++++++\n",i,ao.getState());

    if (i > 38 ){
      EXPECT_EQ(ANIMATION_CALIBRATING, ao.getState());
    }
    else if (i==20 || i==38){
      EXPECT_EQ(ANIMATION_INIT, ao.getState());
    }
    else if ((i>18 && i<20) || i>36){
      EXPECT_EQ(ANIMATION_FINISHED, ao.getState());
    }
    else{
      EXPECT_EQ(ANIMATION_ACTIVE, ao.getState());
    }
  }
}

/*

TEST(AnimationOps_tests, scenario){

Adafruit_TLC5947 tlc = Adafruit_TLC5947();
AnimationList animationList(false);
AnimationOps as(tlc);

AccelStepper acs = AccelStepper();
int pin = 22;
StepperWorker sw = StepperWorker (STEPPER1, acs, pin, false);

LedWorker lw = LedWorker (LED1TOP, 0);

as.addStepperWorker(&sw);
as.addLedWorker(&lw);

int num0 = as.getNumAnimations();
printf("Number of animations: %d\n", num0);

Animation a0;
a0.addKeyFrames({
{LED1TOP, 0, RED, 100},
{LED1TOP, 300, BLUE, 100},
{STEPPER1, 500, 0},
{STEPPER1, 800, 1000},
{STEPPER1, 1200, 2600},

});

int id = as.addAnimation(a0);
printf("Id of animation: %d\n", id);
EXPECT_EQ(num0+1, as.getNumAnimations());
EXPECT_EQ(num0, id);

as.init(SINGLE_ANIMATION, id, false);
EXPECT_EQ(as.getState(), ANIMATION_INIT);

// 11 (1200ms) is the start of the animation
for (int i=0;i<30;i++){

if (i==5) {
test_triggerEndStop(true);
}

if (i==8) {
test_triggerEndStop(false);
}

as.loop();

printf("%d StepperWorkerState: %d\n", i, sw.getState());

if (i < 10){
EXPECT_EQ(as.getState(), ANIMATION_CALIBRATING);
}
else if (i < 23){
EXPECT_EQ(as.getState(), ANIMATION_ACTIVE);
}
else{
EXPECT_EQ(as.getState(), ANIMATION_FINISHED);
}


}
}
*/
