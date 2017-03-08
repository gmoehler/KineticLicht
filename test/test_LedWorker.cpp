#include "test.h"



/*
void integration_test(){

  LedWorker rgb1o = LedWorker (4);

  Animation animation;
  animation.addKeyFrames({
    {LED1TOP, 0, 0},
    {LED1TOP, 1000, RED},
    {LED1TOP, 2000, GREEN},
    {LED1TOP, 4000, BLUE}
  });

  long elapsedTime = 0;
  while(elapsedTime < 5000){
    if (animation.needsTargetFrameUpdate(elapsedTime)) {
      std::vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(elapsedTime);
      for (std::vector<KeyFrame>::iterator kf_it = kfs.begin(); kf_it != kfs.end(); kf_it++) {
        KeyFrame kf = *kf_it;
        switch (kf_it->getId()) {
          case LED1TOP:
          kf.printKeyFrame();
        r  rgb1o.updateTargetKeyFrame(elapsedTime, kf);
          break;
        }
      }
    }

    rgb1o.loop(elapsedTime);
    if (rgb1o.needsUpdate() ){
      RGB rgb1oColor = rgb1o.getColorForUpdate();
      printf("TEST %ld %d %d %d\n", elapsedTime, rgb1oColor.red(), rgb1oColor.green(), rgb1oColor.blue());
    }

    elapsedTime += 200;
  }
*/

  TEST(LedWorker_test, test1){
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
