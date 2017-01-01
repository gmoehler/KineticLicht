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
  int getNumMotorKeyframes(int motorId); 
  void addMotorKeyFrame(int motorId, KeyFrame kf);

  vector<KeyFrameRgb> getRgbKeyframes(int ledId, LedPosition topOrBottom);
  int getNumRgbKeyframes(int ledId, LedPosition topOrBottom);  
  void addRgbKeyFrame(int ledId, KeyFrameRgb kf, LedPosition topOrBottom);

private:
  int getIndex(int ledId, LedPosition topOrBottom);

  map<int, vector<KeyFrame> > motorKeyFrames;
  map<int, vector<KeyFrameRgb> > rgbKeyFrames;
  
};


#endif

