#include "../test/test.h"

void test1(){

  AccelStepper as = AccelStepper();
  int pin = 22;
  StepperWorker sw(1, as, pin, false);

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


int main( int argc, const char* argv[] ){
	test1();

}
