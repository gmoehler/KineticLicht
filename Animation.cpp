#include "Animation.h"

Animation::Animation() {
}

vector<KeyFrame> Animation::getMotorKeyframes(int motorId) {
  vector<KeyFrame> kfs = motorKeyFrames[motorId];
  return kfs;
}

int Animation::getNumMotorKeyframes(int motorId) {
  vector<KeyFrame> kfs = motorKeyFrames[motorId];
  return kfs.size();
}
void Animation::addMotorKeyFrame(int motorId, KeyFrame kf) {
  vector<KeyFrame> kfs = motorKeyFrames[motorId];
  kfs.push_back(kf); 
}

vector<KeyFrameRgb> Animation::getRgbKeyframes(int ledId, LedPosition topOrBottom) {
  int index = getIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  return kfs;
}

int Animation::getNumRgbKeyframes(int ledId, LedPosition topOrBottom) {
  int index = getIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  return kfs.size();
}

void Animation::addRgbKeyFrame(int ledId, KeyFrameRgb kf, LedPosition topOrBottom) {
  int index = getIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  kfs.push_back(kf);
}

int Animation::getIndex(int ledId, LedPosition topOrBottom){
   int index = 2 * (ledId - 1) + 1;
  if (topOrBottom == TOP) {
    index ++;
  }
  return index;
}

