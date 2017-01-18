#include "test.h"

TEST(LedWorker_tests, test1){

  LedWorker rgb1o = LedWorker (4);

  Animation animation;
  animation.addKeyFrames({
    {LED1TOP, 0, 0},
    {LED1TOP, 2000, GREEN},
    {LED2TOP, 2000, GREEN},
    {LED1TOP, 9000, BLUE},
    {LED1TOP, 1500, RED}
  });

  long elapsedTime = 0;
  while(elapsedTime < 10000){
    vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(elapsedTime);
    if (animation.needsTargetFrameUpdate(elapsedTime)) {
      for (vector<KeyFrame>::iterator kf_it = kfs.begin(); kf_it != kfs.end(); kf_it++) {
        KeyFrame kf = *kf_it;
        switch (kf_it->getId()) {
          case LED1TOP:
          rgb1o.updateTargetKeyFrame(elapsedTime, kf);
          break;
        }
      }
    }

    elapsedTime += 200;

  }
}
