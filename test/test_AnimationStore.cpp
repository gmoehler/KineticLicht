#include "test.h"

extern void expectAnimation(KeyFrame kf, int id, long time, long pos, int red, int green, int blue);

TEST(AnimationStore_tests, scenario){
  AnimationStore as;

  AccelStepper acs = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (STEPPER1, acs, pin, false);
  sw.setDebug(true);

  LedWorker lw = LedWorker (LED1TOP);

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
  Adafruit_TLC5947 tlc = Adafruit_TLC5947();

  as.init(tlc, SINGLE, id, false);
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


TEST(AnimationStore_tests, storetest){
  AnimationStore as;

  int num0 = as.getNumAnimations();

  Animation a0;
  a0.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER2, 2000, 1000},
    {STEPPER3, 9000, 2600},
    {LED1TOP, 1500, YELLOW, 50}
  });

  int id = as.addAnimation(a0);
  EXPECT_EQ(num0+1, as.getNumAnimations());
  EXPECT_EQ(num0, id);

  Animation a = as.getAnimation(id);

  // this is the test from test_Animation.cpp

  EXPECT_EQ(4, a.numberOfKeyFrames());

  EXPECT_TRUE(a.needsTargetFrameUpdate(0));
  vector<KeyFrame> kfs =a.getNextTargetKeyFrames(0);

  ASSERT_EQ(1, (int) kfs.size());
  KeyFrame kf = kfs.front();

  EXPECT_EQ(STEPPER1, kf.getId());
  EXPECT_EQ(0, kf.getTimeMs());
  EXPECT_EQ(0, kf.getTargetPosition());
  EXPECT_EQ(0, kf.getTargetColor().red());
  EXPECT_EQ(0, kf.getTargetColor().green());
  EXPECT_EQ(0, kf.getTargetColor().blue());

  EXPECT_TRUE(a.needsTargetFrameUpdate(1000));
  kfs =a.getNextTargetKeyFrames(1000);
  ASSERT_EQ(1, (int) kfs.size());
  kf = kfs.front();

  EXPECT_EQ(LED1TOP, kf.getId());
  EXPECT_EQ(1500, kf.getTimeMs());
  EXPECT_EQ(0, kf.getTargetPosition());
  EXPECT_EQ(RGB_MAX_VAL/2,kf.getTargetColor().red());
  EXPECT_EQ(RGB_MAX_VAL/2,kf.getTargetColor().green());
  EXPECT_EQ(0, kf.getTargetColor().blue());

  EXPECT_TRUE(a.needsTargetFrameUpdate(1600));
  kfs =a.getNextTargetKeyFrames(1600);
  ASSERT_EQ(1, (int)kfs.size());
  kf = kfs.front();

  EXPECT_EQ(STEPPER2, kf.getId());
  EXPECT_EQ(2000, kf.getTimeMs());
  EXPECT_EQ(1000, kf.getTargetPosition());
  EXPECT_EQ(0, kf.getTargetColor().red());
  EXPECT_EQ(0, kf.getTargetColor().green());
  EXPECT_EQ(0, kf.getTargetColor().blue());

  EXPECT_FALSE(a.isAnimationFinished());

  EXPECT_TRUE(a.needsTargetFrameUpdate(2100));
  kfs =a.getNextTargetKeyFrames(2100);
  //a.printAnimation();
  EXPECT_TRUE(a.needsTargetFrameUpdate(10000));
  EXPECT_TRUE(a.isAnimationFinished());

}
