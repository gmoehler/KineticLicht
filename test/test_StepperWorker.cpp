#include "test.h"

TEST(StepperWorker_test, test1){
  AccelStepper as = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (as, 1, pin, false);

  Animation animation;
  animation.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER1, 1000, 100}
  });

  vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(0);
  ASSERT_EQ(1,(int) kfs.size());
  sw.updateTargetKeyFrame(0, kfs[0]);

  kfs = animation.getNextTargetKeyFrames(200);
  ASSERT_EQ(1,(int)kfs.size());
  sw.updateTargetKeyFrame(0, kfs[0]);

  sw.loop(200);
  EXPECT_EQ(100, as.test_getSpeed());
  EXPECT_EQ(StepperWorkerState::ACTIVE, sw.getState());

  sw.loop(1251); // we allow for 250ms overshoot time
  EXPECT_EQ(1, as.test_getSpeed()); // stopped: we use speed 1 instead of 0 due to lib bug
  EXPECT_EQ(StepperWorkerState::PAST_TARGET, sw.getState());
}

TEST(StepperWorker_test, endstopTest){
  AccelStepper as = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (as, 1, pin, false);

  Animation animation;
  animation.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER1, 1000, 100}
  });

  vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(0);
  ASSERT_EQ(1,(int) kfs.size());
  sw.updateTargetKeyFrame(0, kfs[0]);

  kfs = animation.getNextTargetKeyFrames(200);
  ASSERT_EQ(1,(int)kfs.size());
  sw.updateTargetKeyFrame(0, kfs[0]);

  sw.loop(200);
  int numResets = test_getNumEndpointResets();
  test_triggerEndStop();
  sw.loop(300);
  EXPECT_EQ(numResets+1, test_getNumEndpointResets());
}

TEST(StepperWorker_test, calibrationTest){
  //TODO
}
