#include "../test/test.h"

typedef unsigned int animation_as_uint_t [8];

animation_as_uint_t* allV0[4];
int rows0 = 0;

animation_as_uint_t* getAnis0(int id){
  unsigned v[][8] = {
    {LED1BOT, 20, 0, RED, 100, 2},
    {LED1TOP, 20, 0, BLACK, 0, 2}
  };

  rows0 = sizeof(v) / sizeof(v[0]);
  auto v_heap = new unsigned[rows0][8]();
  std::copy(&v[0][0], &v[0][0]+rows0*8,&v_heap[0][0]);
  int rows01 = sizeof(v_heap) / sizeof(v_heap[0]);

  EXPECT_EQ(rows0, rows01);

  for (int i=0; i<rows0; i++){
    for (int j=0; j<8; j++){
      printf("%u ", v[i][j]);
    }
    printf("\n");
  }
  for (int i=0; i<rows0; i++){
    for (int j=0; j<8; j++){
      printf("%u ", v_heap[i][j]);
    }
    printf("\n");
  }

  allV0[0] = v_heap;

  for (int i=0; i<rows0; i++){
    for (int j=0; j<8; j++){
      printf("%u ", allV0[0][i][j]);
    }
    printf("\n");
  }

  return allV0[id];
}

void cleanupAnis0(){
  for (int i=0; i<4; i++){
    delete[] allV0[i];
  }
}


void test1()
{
  animation_as_uint_t* as = getAnis0(0);

  Adafruit_TLC5947 tlc = Adafruit_TLC5947();
  AnimationOps ao(tlc, true);

  int num0 = ao.getNumAnimations();
  EXPECT_EQ(4, num0);

  for (int i=0; i<rows0; i++){
    for (int j=0; j<8; j++){
      printf("%u ",as[i][j]);
    }
    printf("\n");
  }

  cleanupAnis0();

  printf ("reading...\n");

  AnimationList al(true);

  animation_as_uint_t* ani = al.getAnimationAsUint(0);

  for (int i=0; i<3; i++){
    for (int j=0; j<8; j++){
      printf("%u ",ani[i][j]);
    }
    printf("\n");
    KeyFrame kf(ani[i]);
    kf.printKeyFrame();
  }

  //animation_as_uint_t *a;
  //a = new animation_as_uint_t[4];

  int numKf = al.getNumKeyFrames(0);
  Animation animation(ani, numKf);
  EXPECT_EQ(numKf, animation.numberOfKeyFrames());
  //animation.printAnimation();

  printf ("current animation...\n");
  ao.selectAnimation(1);
  Animation& an0 = ao._getCurrentAnimation();
  EXPECT_EQ(56, an0.numberOfKeyFrames());
  //an0.printAnimation();

}

void test2(){
	Adafruit_TLC5947 tlc = Adafruit_TLC5947();
    AnimationOps ao(tlc, true);
	EXPECT_EQ(4, ao.getNumAnimations());

	ao.init(SINGLE_ANIMATION, 3, false);
	Animation& a = ao._getCurrentAnimation();
    EXPECT_EQ(35, a.numberOfKeyFrames());
    a.printAnimation();

    printf("+++++++++++++++\n");

    printf("Num keys: %d\n", a.numberOfKeyFrames());
    //for (unsigned i=0; i<a.numberOfKeyFrames(); i++){
    for (unsigned i=0; i<3; i++){
      printf("%d", i);
      KeyFrame kf = a.getKeyFrame(i);
      kf.printKeyFrame();
  }
  printf("+++++++++++++++\n");
}

void test3(){

  Adafruit_TLC5947 tlc = Adafruit_TLC5947();
  AnimationOps ao(tlc, true);
  EXPECT_EQ(4, ao.getNumAnimations());

  AccelStepper acs = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (STEPPER1, acs, pin, false);
  sw.setDebug(true);

  LedWorker lw = LedWorker (LED1TOP, 0);

  ao.addStepperWorker(&sw);
  ao.addLedWorker(&lw);

  ao.init(SINGLE_ANIMATION, 0, false);
  EXPECT_EQ(ao.getState(), ANIMATION_INIT);

  for (int i=0;i<20;i++){
    ao.loop();
    printf("%d +++++++%d++++++++\n",i,ao.getState());
  }
}

int main( int argc, const char* argv[] ){
	test1();
	//test2();
	test3();

}
