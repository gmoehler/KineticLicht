#include "test.h"

void expectAnimation(KeyFrame kf, int id, long time, long pos, int red, int green, int blue) {
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
    {STEPPER2, 2000, 1000},
    {STEPPER3, 9000, 2600},
    {LED1TOP, 1500, YELLOW, 50}
  });

  //a.printAnimation();

  EXPECT_EQ(4, a.numberOfKeyFrames());
  EXPECT_EQ(true,a.containsMotorFrames());


  EXPECT_TRUE(a.needsTargetFrameUpdate(0));
  std::vector<KeyFrame> kfs =a.getNextTargetKeyFrames(0);

  ASSERT_EQ(1, (int) kfs.size());
  KeyFrame kf = kfs.front();
  expectAnimation(kf, STEPPER1, 0,0,0,0,0);

  EXPECT_TRUE(a.needsTargetFrameUpdate(1000));
  kfs =a.getNextTargetKeyFrames(1000);
  ASSERT_EQ(1, (int) kfs.size());
  kf = kfs.front();
  expectAnimation(kf, LED1TOP, 1500,0,RGB_MAX_VAL/2,RGB_MAX_VAL/2,0);

  EXPECT_TRUE(a.needsTargetFrameUpdate(1600));
  kfs =a.getNextTargetKeyFrames(1600);
  ASSERT_EQ(1, (int)kfs.size());
  kf = kfs.front();
  expectAnimation(kf, STEPPER2, 2000, 1000, 0, 0, 0);

  EXPECT_FALSE(a.isAnimationFinished());

  EXPECT_TRUE(a.needsTargetFrameUpdate(2100));
  kfs =a.getNextTargetKeyFrames(2100);
  //a.printAnimation();
  EXPECT_TRUE(a.needsTargetFrameUpdate(10000));
  EXPECT_TRUE(a.isAnimationFinished());

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
