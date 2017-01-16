#include "test.h"

TEST(Animation_tests, test1){
  Animation a;

  a.addKeyFrames({
    {STEPPER1, 0, 0},
    {LED1TOP, 1500, YELLOW, 50},
    {STEPPER2, 2000, 1000},
    {STEPPER3, 9000, 2600}
  });

  EXPECT_EQ(4, a.numberOfKeyFrames());

  EXPECT_TRUE(a.hasNextTargetKeyFrame(0));
    vector<KeyFrame> kfs =a.getNextTargetKeyFrames(0);

    ASSERT_EQ(1, (int) kfs.size());
    KeyFrame kf = kfs.front();

    EXPECT_EQ(STEPPER1, kf.getId());
    EXPECT_EQ(0, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());

  EXPECT_TRUE(a.hasNextTargetKeyFrame(1000));
  kfs =a.getNextTargetKeyFrames(1000);
  ASSERT_EQ(1, (int) kfs.size());
  kf = kfs.front();

    EXPECT_EQ(LED1TOP, kf.getId()); EXPECT_EQ(1500, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition()); EXPECT_EQ(RGB_MAX_VAL,
    kf.getTargetColor().red()); EXPECT_EQ(RGB_MAX_VAL,
    kf.getTargetColor().green()); EXPECT_EQ(0, kf.getTargetColor().blue());

 EXPECT_TRUE(a.hasNextTargetKeyFrame(1600));
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

  EXPECT_TRUE(a.hasNextTargetKeyFrame(2100));
   kfs =a.getNextTargetKeyFrames(2100);
   EXPECT_FALSE(a.hasNextTargetKeyFrame(10000));
   EXPECT_TRUE(a.isAnimationFinished());

  }
