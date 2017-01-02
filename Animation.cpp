#include "Animation.h"

Animation::Animation() {
  // nothing to be done
}

vector<KeyFrame> Animation::getMotorKeyframes(int motorId) {
  vector<KeyFrame> kfs = motorKeyFrames[motorId];
  return kfs;
}

void Animation::addMotorKeyFrame(int motorId, KeyFrame new_kf) {
  vector<KeyFrame> kfs = motorKeyFrames[motorId];
  kfs.push_back(new_kf);
}

void Animation::addMotorKeyFrames(int motorId, vector<KeyFrame> new_kfs) {
  vector<KeyFrame> kfs = motorKeyFrames[motorId];
  kfs.insert(kfs.end(), new_kfs.begin(), new_kfs.end());
}

void Animation::addMotorKeyFrames(vector<KeyFrame> id_kfs) {

  for (vector<KeyFrame>::iterator it = id_kfs.begin(); it != id_kfs.end(); ++it)
  {
    int motorId =  it->getId();
    addMotorKeyFrame(motorId, *it);
  }
}

vector<KeyFrameRgb> Animation::getRgbKeyframes(int ledId, LedPosition topOrBottom) {
  int index = getLedIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  return kfs;
}

void Animation::addRgbKeyFrame(int ledId, KeyFrameRgb new_kf, LedPosition topOrBottom) {
  int index = getLedIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  kfs.push_back(new_kf);
}

void Animation::addRgbKeyFrames(int ledId, vector<KeyFrameRgb> new_kfs, LedPosition topOrBottom) {
  int index = getLedIndex(ledId, topOrBottom);
  vector<KeyFrameRgb> kfs = rgbKeyFrames[index];
  kfs.insert(kfs.end(), new_kfs.begin(), new_kfs.end());
}

int Animation::getLedIndex(int ledId, LedPosition topOrBottom) {
  int index = 2 * (ledId - 1) + 1;
  if (topOrBottom == TOP) {
    index ++;
  }
  return index;
}

