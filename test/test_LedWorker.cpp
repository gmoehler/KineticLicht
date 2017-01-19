#include "test.h"

TEST(LedWorker_tests, test1){

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
    vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(elapsedTime);
    for (vector<KeyFrame>::iterator kf_it = kfs.begin(); kf_it != kfs.end(); kf_it++) {
        KeyFrame kf = *kf_it;
        switch (kf_it->getId()) {
          case LED1TOP:
          kf.printKeyFrame();
          rgb1o.updateTargetKeyFrame(elapsedTime, kf);
          break;
        }
      }
    }

    rgb1o.loop(elapsedTime);
    if (rgb1o.needsUpdate() ){
    	RGB rgb1oColor = rgb1o.getColorForUpdate();
    	printf("%ld %d %d %d\n", elapsedTime, rgb1oColor.red(), rgb1oColor.green(), rgb1oColor.blue());
    }

    elapsedTime += 200;
  }

}
