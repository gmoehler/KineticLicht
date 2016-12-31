#ifndef ANIMATION_H
#define ANIMATION_H

#include "KeyFrame.h"

enum LedPosition { TOP, BOTTOM };

class Animation
{
public:

  Animation();

  KeyFrame* getMotorKeyframes(int motorId);
  int getNumMotorKeyframes(int motorId); 
  void addMotorKeyFrames(int motorId, KeyFrame&   kf, int numKeyFrames);

  KeyFrameRgb* getRgbKeyframes(int ledId, LedPosition topOrBottom);
  int getNumRgbKeyframes(int ledId, LedPosition topOrBottom);  
  void addRgbKeyFrames(int ledId, KeyFrameRgb& kf, LedPosition topOrBottom, int numKeyFrames);

private:
  int getIndex(int ledId, LedPosition topOrBottom);

  KeyFrame motorKeyFrames [][4] ;
  int numMotorFrames[4];
  KeyFrameRgb rgbKeyFrames [][8] ;
  int numRgbFrames[8];
    
};


#endif

