#include "../test/test.h"

void test1(){

	LedWorker lw = LedWorker (4,4);

    Animation animation;
    animation.addKeyFrames({
      {LED1TOP, 0, 0},
      {LED1TOP, 1000, RED}
    });

    std::vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(0);
    ASSERT_EQ(1,(int) kfs.size());
    lw.updateTargetKeyFrame(0, kfs[0]);
    RGB rgb = lw.getColorForUpdate();
    expect_rgb(rgb, 0, 0, 0);

    kfs = animation.getNextTargetKeyFrames(200);
    ASSERT_EQ(1,(int)kfs.size());
    lw.updateTargetKeyFrame(0, kfs[0]);

    lw.loop(200);
    rgb = lw.getColorForUpdate();
    expect_rgb(rgb, 818, 0, 0);

    lw.loop(1000);
    rgb = lw.getColorForUpdate();
    expect_rgb(rgb, RGB_MAX_VAL, 0, 0);
    EXPECT_FALSE(lw.hasPassedTargetKeyFrame());

    lw.loop(1001);
    EXPECT_TRUE(lw.hasPassedTargetKeyFrame());


}


int main( int argc, const char* argv[] ){
	test1();

}
