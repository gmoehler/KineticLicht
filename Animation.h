#ifndef ANIMATION_H
#define ANIMATION_H

#include <ArduinoSTL.h>
#include <map>

#include "KeyFrame.h"

using namespace std;

enum LedPosition { TOP, BOTTOM };

class Animation
{
public:

  Animation();

  vector<KeyFrame> getMotorKeyframes(int motorId);
  // add single key frames
  void addMotorKeyFrame(int motorId, KeyFrame new_kf);
  // add many key frames for one motor
  void addMotorKeyFrames(int motorId, vector<KeyFrame> new_kfs);
  // add many key frames for arbitrary motors defined as first value
  void addMotorKeyFrames(vector<KeyFrame> new_kfs);

  vector<KeyFrameRgb> getRgbKeyframes(int ledId, LedPosition topOrBottom);
  void addRgbKeyFrame(int ledId, KeyFrameRgb new_kf, LedPosition topOrBottom);
  void addRgbKeyFrames(int ledId, vector<KeyFrameRgb> new_kfs, LedPosition topOrBottom);

private:
  int getLedIndex(int ledId, LedPosition topOrBottom);

  std:: map<int, vector<KeyFrame> > motorKeyFrames;
  std:: map<int, vector<KeyFrameRgb> > rgbKeyFrames;
  
};


#endif

