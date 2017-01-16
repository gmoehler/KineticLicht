#include "stdio.h"
#include "gtest/gtest.h"
#include "../src/KeyFrame.h"
#include "../src/Animation.h"

TEST(Animation_tests, test1){
  Animation a;

  a.addKeyFrames({
    {STEPPER1, 0, 0},
    {LED1TOP, 1500, YELLOW, 50},
    {STEPPER1, 2000, 1000},
    {STEPPER1, 9000, 2600},
    {STEPPER1, 12000, 2600},
    {STEPPER1, 26000, 10}
  });
  
  EXPECT_EQ(true, a.hasNextTargetKeyFrame());
  
  KeyFrame kf =a.getNextTargetKeyFrame();
  
    EXPECT_EQ(STEPPER1, kf.getId());
    EXPECT_EQ(0, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());

  KeyFrame kf =a.getNextTargetKeyFrame();
  
    EXPECT_EQ(LED1TOP, kf.getId());
    EXPECT_EQ(1500, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(RGB_MAX_VAL, kf.getTargetColor().red());
    EXPECT_EQ(RGB_MAX_VAL, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());
   
 KeyFrame kf =a.getNextTargetKeyFrame();
  
    EXPECT_EQ(STEPPER1, kf.getId());
    EXPECT_EQ(2000, kf.getTimeMs());
    EXPECT_EQ(1000, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());


  }
