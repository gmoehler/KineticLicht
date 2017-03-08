#ifndef test_h
#define test_h

#include "stdio.h"
#include "gtest/gtest.h"

#include "mock_Arduino.h"

#include <KeyFrame.h>
#include <Animation.h>
#include <AnimationOps.h>
#include <LedWorker.h>
#include <StepperWorker.h>
#include <FiniteStates.h>

int compressExpand(int val);
void RGB_EXPECT_EQ(int a, int b);
void expect_rgb(RGB rgb, int r, int g, int b);

#endif
