#include "Animation.h"

Animation::Animation() {

}

KeyFrame* Animation::getMotorKeyframes(int motorId) {
  return motorKeyFrames[motorId - 1];
}

int Animation::getNumMotorKeyframes(int motorId) {
  return numMotorFrames[motorId];
}
void Animation::addMotorKeyFrames(int motorId, KeyFrame&  kf, int numKeyFrames) {
  motorKeyFrames[motorId - 1] = kf;
  numMotorFrames[motorId - 1] = numKeyFrames; 
}

KeyFrameRgb* Animation::getRgbKeyframes(int ledId, LedPosition topOrBottom) {
  return rgbKeyFrames[getIndex(ledId, topOrBottom)];
}

int Animation::getNumRgbKeyframes(int ledId, LedPosition topOrBottom) {
  return numRgbFrames[getIndex(ledId, topOrBottom)];
}

void Animation::addRgbKeyFrames(int ledId, KeyFrameRgb& kf, LedPosition topOrBottom, int numKeyFrames) {
  rgbKeyFrames[getIndex(ledId, topOrBottom)] = kf;
  numRgbFrames[getIndex(ledId, topOrBottom)] = numKeyFrames;
}

int Animation::getIndex(int ledId, LedPosition topOrBottom){
   int index = 2 * (ledId - 1) + 1;
  if (topOrBottom == TOP) {
    index ++;
  }
  return index;
}

