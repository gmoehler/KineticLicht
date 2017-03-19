#include "test.h"

void expectKeyFrame(KeyFrame kf, int id, long time, long pos, int red, int green, int blue) {
  EXPECT_EQ(id, kf.getId());
  EXPECT_EQ(time, kf.getTimeMs());
  EXPECT_EQ(pos, kf.getTargetPosition());
  RGB_EXPECT_EQ(red, kf.getTargetColor().red());
  RGB_EXPECT_EQ(green, kf.getTargetColor().green());
  RGB_EXPECT_EQ(blue, kf.getTargetColor().blue());
}

TEST(Animation_tests, test1){
  Animation a;

  a.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER2, 0, 0},
    {STEPPER3, 0, 0},
    {LED1TOP, 0, BLUE, 50},
    {STEPPER2, 2000, 1000},
    {STEPPER3, 9000, 2600},
    {STEPPER3, 10000, 2600},
    {LED1TOP, 1500, YELLOW, 50}
  });

  a.printAnimation();

  EXPECT_EQ(8, a.numberOfKeyFrames());
  EXPECT_EQ(true,a.containsMotorFrames());


  //initial values
  std::vector<KeyFrame> kfs =a.getNextTargetKeyFrames(0);
  ASSERT_EQ(4, (int) kfs.size());

  kfs =a.getNextTargetKeyFrames(10);
  ASSERT_EQ(3, (int) kfs.size());

  //EXPECT_TRUE(a.needsTargetFrameUpdate(1000));
  kfs = a.getNextTargetKeyFrames(9001);
  ASSERT_EQ(1, (int) kfs.size());

  KeyFrame   kf = kfs.front();
  expectKeyFrame(kf, STEPPER3, 10000, 2600, 0, 0, 0);

  kfs = a.getNextTargetKeyFrames(10001);
  EXPECT_TRUE(a.isAnimationFinished(10001));

}

TEST(Animation_tests, noMotorFrames){
  Animation a;

  a.addKeyFrames({
    {LED1TOP, 1500, YELLOW, 50}
  });

  //a.printAnimation();

  EXPECT_EQ( 1, a.numberOfKeyFrames());
  EXPECT_FALSE(a.containsMotorFrames());

  }

  TEST(Animation_tests, sorting1){
    Animation a;

    a.addKeyFrames({
      {STEPPER1, 0, 0},
      {STEPPER2, 1000, 2100},
      {STEPPER2, 2000, 2200},
      {STEPPER1, 3000, 1100},
      {STEPPER1, 10000, 1200},
      {STEPPER1, 10000, 1300},
    });

    //a.printAnimation();

    //EXPECT_TRUE(a.needsTargetFrameUpdate(5));
    std::vector<KeyFrame> kfs  = a.getNextTargetKeyFrames(0);
    /*for (unsigned i=0; i < kfs.size(); i++){
      kfs[i].printKeyFrame();
    }*/
    ASSERT_EQ(2u, kfs.size());

    //a.printAnimation();
    kfs  = a.getNextTargetKeyFrames(5);
    /*for (unsigned i=0; i < kfs.size(); i++){
      kfs[i].printKeyFrame();
    }*/

    a.printAnimation();
    // up to second kf for each of the 2 steppers
    EXPECT_EQ(1u, kfs.size());

    kfs  = a.getNextTargetKeyFrames(1001);
    /*for (unsigned i=0; i < kfs.size(); i++){
      kfs[i].printKeyFrame();
    }*/

    //a.printAnimation();
    // up to second kf for each of the 2 steppers
    EXPECT_EQ(1u, kfs.size());

}
