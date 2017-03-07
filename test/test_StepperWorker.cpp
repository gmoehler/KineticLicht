#include "test.h"

TEST(StepperWorker_test, active_past_target){
  AccelStepper as = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (1, as, pin, false);

  Animation animation;
  animation.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER1, 1000, 100}
  });

  long elapsedtime = 0;
  std::vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(elapsedtime);
  ASSERT_EQ(1,(int) kfs.size());
  sw.updateTargetKeyFrame(elapsedtime, kfs[0]);

  elapsedtime = 10;
  kfs = animation.getNextTargetKeyFrames(elapsedtime);
  ASSERT_EQ(1,(int)kfs.size());
  sw.updateTargetKeyFrame(elapsedtime, kfs[0]);

  ASSERT_EQ(INIT, sw.getState());

  elapsedtime = 100;
  printf("time: %ld\n", elapsedtime);
  sw.loop(elapsedtime);
  // still in INIT state
  ASSERT_EQ(INIT, sw.getState());
  sw.startAnimation();

  elapsedtime = 200;
  printf("time: %ld\n", elapsedtime);
  sw.loop(elapsedtime);
  ASSERT_EQ(ACTIVE, sw.getState());

  // speed = 100 / (1000-200)
  EXPECT_EQ(125, as.test_getSpeed());
  EXPECT_EQ(StepperWorkerState::ACTIVE, sw.getState());

  elapsedtime=1100;
  printf("time: %ld\n", elapsedtime);
  sw.loop(elapsedtime);
  sw.loop(elapsedtime);
  EXPECT_EQ(125, as.test_getSpeed());
  // we allow for 250ms overshoot time, so still active
  EXPECT_EQ(StepperWorkerState::ACTIVE, sw.getState());

  elapsedtime=1251; // we allow for 250ms overshoot time
  sw.loop(elapsedtime);
  printf("time: %ld", elapsedtime);
  sw.loop(elapsedtime);
  EXPECT_EQ(1, as.test_getSpeed()); // stopped: we use speed 1 instead of 0 due to lib bug
  EXPECT_EQ(StepperWorkerState::PAST_TARGET, sw.getState());
}

TEST(StepperWorker_test, endstopTest){
  AccelStepper as = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (1, as, pin, false);

  Animation animation;
  animation.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER1, 1000, 100},
    {STEPPER1, 2000, 0},
    {STEPPER1, 3000, 100},
  });

  long elapsedtime = 0;
  printf("time: %ld\n", elapsedtime);
  std::vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(elapsedtime);
  ASSERT_EQ(1,(int) kfs.size());
  sw.updateTargetKeyFrame(elapsedtime, kfs[0]);

  elapsedtime = 100;
  printf("time: %ld\n", elapsedtime);
  kfs = animation.getNextTargetKeyFrames(elapsedtime);
  ASSERT_EQ(1,(int) kfs.size());
  sw.updateTargetKeyFrame(elapsedtime, kfs[0]);
  sw.startAnimation();
  sw.loop(elapsedtime);

  elapsedtime = 1001;
  printf("time: %ld\n", elapsedtime);
  kfs = animation.getNextTargetKeyFrames(elapsedtime);
  ASSERT_EQ(1,(int)kfs.size());
  sw.updateTargetKeyFrame(elapsedtime, kfs[0]);
  sw.loop(elapsedtime);

  test_triggerEndStop(true);
  elapsedtime = 1300;
  printf("time: %ld\n", elapsedtime);
  sw.loop(elapsedtime);
  EXPECT_EQ(StepperWorkerState::ENDSTOP_HIT, sw.getState());

  test_triggerEndStop(false);
  elapsedtime = 1599;
  printf("time: %ld\n", elapsedtime);
  sw.loop(elapsedtime);
  // 300ms have not passed yet
  EXPECT_EQ(StepperWorkerState::ENDSTOP_HIT, sw.getState());

  elapsedtime = 1601;
  printf("time: %ld\n", elapsedtime);
  sw.loop(elapsedtime);
  // now we should be back waiting for a pos.  speed
  EXPECT_EQ(StepperWorkerState::ENDSTOP_WAITING, sw.getState());

  // test transition to active, when speed gets positive again
  elapsedtime = 2100;
  printf("time: %ld\n", elapsedtime);
  kfs = animation.getNextTargetKeyFrames(elapsedtime);
  ASSERT_EQ(1,(int)kfs.size());
  sw.updateTargetKeyFrame(elapsedtime, kfs[0]);

  sw.loop(elapsedtime);
  // now we should be back in active state
  EXPECT_EQ(StepperWorkerState::ACTIVE, sw.getState());

}

TEST(StepperWorker_test, calibrationTest){
  AccelStepper as = AccelStepper();
  int pin = 22;
  StepperWorker sw = StepperWorker (1, as, pin, false);

  Animation animation;
  animation.addKeyFrames({
    {STEPPER1, 0, 0},
    {STEPPER1, 1000, 100}
  });

  long elapsedtime = 0;
  printf("time: %ld\n", elapsedtime);
  std::vector<KeyFrame> kfs = animation.getNextTargetKeyFrames(elapsedtime);
  ASSERT_EQ(1,(int) kfs.size());
  sw.updateTargetKeyFrame(elapsedtime, kfs[0]);

  //start calibration ... keep time 0
  printf("time: %ld\n", elapsedtime);
  EXPECT_EQ(StepperWorkerState::INIT, sw.getState());

  sw.startCalibration();
  sw.loop(elapsedtime);
  EXPECT_EQ(StepperWorkerState::CALIBRATING_UP, sw.getState());

  test_triggerEndStop(true);
  sw.loop(elapsedtime);
  EXPECT_EQ(StepperWorkerState::CALIBRATING_ENDSTOPHIT, sw.getState());

  test_triggerEndStop(false);
  sw.loop(elapsedtime);
  // still below 300ms
  EXPECT_EQ(StepperWorkerState::CALIBRATING_ENDSTOPHIT, sw.getState());

  elapsedtime = 301;
  sw.loop(elapsedtime);
  printf("time: %ld\n", elapsedtime);
  EXPECT_EQ(StepperWorkerState::CALIBRATION_FINISHED, sw.getState());

 //start animation (time reset)
  elapsedtime = 100;
  printf("time: %ld\n", elapsedtime);
  kfs = animation.getNextTargetKeyFrames(elapsedtime);
  ASSERT_EQ(1,(int) kfs.size());
  sw.updateTargetKeyFrame(elapsedtime, kfs[0]);
  sw.startAnimation();
  sw.loop(elapsedtime);
  EXPECT_EQ(StepperWorkerState::ACTIVE, sw.getState());
}
