#include "test.h"

TEST(KeyFrame_tests, test1){

//    KeyFrame kf = KeyFrame(42, 3333, 2000, 1000, 1000);
    KeyFrame kf = KeyFrame();
    EXPECT_EQ((unsigned) 0, kf.getId());
    EXPECT_EQ(0, kf.getTimeMs());
    EXPECT_EQ((unsigned)0, kf.getTargetPosition());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().red());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().green());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().blue());
    //EXPECT_EQ(NOFRAMETYPE, kf.getType());

    kf = KeyFrame(42, 3333, 1000);
    EXPECT_EQ((unsigned)42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ((unsigned)1000, kf.getTargetPosition());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().red());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().green());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().blue());
    EXPECT_EQ(MOTOR, kf.getType());

    unsigned v1[] = {42, 33, 1000, 0, 0, 0, 0, 1};
    kf = KeyFrame(v1);
    EXPECT_EQ((unsigned)42, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ((unsigned)1000, kf.getTargetPosition());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().red());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().green());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().blue());
    EXPECT_EQ(MOTOR, kf.getType());

    kf = KeyFrame(42, 3333, 1000, 2000, 3000);
    EXPECT_EQ((unsigned)42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ((unsigned)0, kf.getTargetPosition());
    EXPECT_EQ((unsigned)1000, kf.getTargetColor().red());
    EXPECT_EQ((unsigned)2000, kf.getTargetColor().green());
    EXPECT_EQ((unsigned)3000, kf.getTargetColor().blue());
    EXPECT_EQ(RGB_LED, kf.getType());

    unsigned v2[] = {42, 33, 0, 1000, 2000, 3000, 100, 2};
    kf = KeyFrame(v2);
    EXPECT_EQ((unsigned)42, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ((unsigned)0, kf.getTargetPosition());
    EXPECT_EQ((unsigned)1000, kf.getTargetColor().red());
    EXPECT_EQ((unsigned)2000, kf.getTargetColor().green());
    EXPECT_EQ((unsigned)3000, kf.getTargetColor().blue());
    EXPECT_EQ(RGB_LED, kf.getType());

    kf = KeyFrame(42, 3333, 1000, 2000, 3000, 50);
    EXPECT_EQ((unsigned)42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ((unsigned)0, kf.getTargetPosition());
    EXPECT_EQ((unsigned)500, kf.getTargetColor().red());
    EXPECT_EQ((unsigned)1000, kf.getTargetColor().green());
    EXPECT_EQ((unsigned)1500, kf.getTargetColor().blue());
    EXPECT_EQ(RGB_LED, kf.getType());

    kf = KeyFrame(42, 3333, RGB::rgb_yellow());
    EXPECT_EQ((unsigned)42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ((unsigned)0, kf.getTargetPosition());
    EXPECT_EQ((unsigned)RGB_MAX_VAL, kf.getTargetColor().red());
    EXPECT_EQ((unsigned)RGB_MAX_VAL, kf.getTargetColor().green());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().blue());
    EXPECT_EQ(RGB_LED, kf.getType());

    kf = KeyFrame(42, 3333, 1000);
    EXPECT_EQ((unsigned)42, kf.getId());
    EXPECT_EQ(3333, kf.getTimeMs());
    EXPECT_EQ((unsigned)1000, kf.getTargetPosition());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().red());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().green());
    EXPECT_EQ((unsigned)0, kf.getTargetColor().blue());
    EXPECT_EQ(MOTOR, kf.getType());
  }
