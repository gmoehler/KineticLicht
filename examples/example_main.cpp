#include "../test/test.h"

void test1(){
	KeyFrame kf = KeyFrame();
    EXPECT_EQ( NO_ACTIVATOR, kf.getId());
    EXPECT_EQ(0, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());
    EXPECT_EQ(NOFRAMETYPE, kf.getType());

    kf = KeyFrame(STEPPER1, 3300, 1000);
    EXPECT_EQ(STEPPER1, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ(1000, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());
    EXPECT_EQ(MOTORFRAME, kf.getType());

    unsigned v1[] = {STEPPER1, 33, 1000, 0, 0, 0, 0};
    kf = KeyFrame(v1);
    EXPECT_EQ(STEPPER1, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ(1000, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());
    EXPECT_EQ(MOTORFRAME, kf.getType());

    kf = KeyFrame(LED1BOT, 3300, 1000, 2000, 3000);
    EXPECT_EQ(LED1BOT, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(1000, kf.getTargetColor().red());
    EXPECT_EQ(2000, kf.getTargetColor().green());
    EXPECT_EQ(3000, kf.getTargetColor().blue());
    EXPECT_EQ(LEDFRAME, kf.getType());

    unsigned v2[] = {LED1BOT, 33, 0, 1000, 2000, 3000, 100};
    kf = KeyFrame(v2);
    EXPECT_EQ(LED1BOT, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(1000, kf.getTargetColor().red());
    EXPECT_EQ(2000, kf.getTargetColor().green());
    EXPECT_EQ(3000, kf.getTargetColor().blue());
    EXPECT_EQ(LEDFRAME, kf.getType());

    kf = KeyFrame(LED1BOT, 3300, 1000, 2000, 3000, 50);
    EXPECT_EQ(LED1BOT, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(500, kf.getTargetColor().red());
    EXPECT_EQ(1000, kf.getTargetColor().green());
    EXPECT_EQ(1500, kf.getTargetColor().blue());
    EXPECT_EQ(LEDFRAME, kf.getType());

    kf = KeyFrame(LED1BOT, 3300, RGB::rgb_yellow());
    EXPECT_EQ(LED1BOT, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ(0, kf.getTargetPosition());
    EXPECT_EQ(RGB_MAX_VAL, kf.getTargetColor().red());
    EXPECT_EQ(RGB_MAX_VAL, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());
    EXPECT_EQ(LEDFRAME, kf.getType());

    kf = KeyFrame(STEPPER1, 3300, 1000);
    EXPECT_EQ(STEPPER1, kf.getId());
    EXPECT_EQ(3300, kf.getTimeMs());
    EXPECT_EQ(1000, kf.getTargetPosition());
    EXPECT_EQ(0, kf.getTargetColor().red());
    EXPECT_EQ(0, kf.getTargetColor().green());
    EXPECT_EQ(0, kf.getTargetColor().blue());
    EXPECT_EQ(MOTORFRAME, kf.getType());
  }


int main( int argc, const char* argv[] ){
	test1();

}
