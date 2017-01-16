#include "stdio.h"
#include "gtest/gtest.h"
#include "../src/KeyFrame.h"

TEST(KeyFrame_tests, test1){

//    KeyFrame kf = KeyFrame(42, 3333, 2000, 1000, 1000);
    KeyFrame kf = KeyFrame();
    EXPECT_EQ(0, kf.getId());
    EXPECT_EQ(0, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());

    kf = KeyFrame(42, 3333, 1000);
    EXPECT_EQ(42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ(1000, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());

    kf = KeyFrame(42, 3333, 1000, 2000, 3000);
    EXPECT_EQ(42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(1000, kf.getTargetColor().red());
    EXPECT_EQ(2000, kf.getTargetColor().green());
    EXPECT_EQ(3000, kf.getTargetColor().blue());

    kf = KeyFrame(42, 3333, 1000, 2000, 3000, 50);
    EXPECT_EQ(42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(500, kf.getTargetColor().red());
    EXPECT_EQ(1000, kf.getTargetColor().green());
    EXPECT_EQ(1500, kf.getTargetColor().blue());

    kf = KeyFrame(42, 3333, RGB::rgb_yellow());
    EXPECT_EQ(42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(RGB_MAX_VAL, kf.getTargetColor().red());
    EXPECT_EQ(RGB_MAX_VAL, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());

    kf = KeyFrame(42, 3333, 1000);
    EXPECT_EQ(42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ(1000, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());

  }
