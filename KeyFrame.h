#ifndef KeyFrame_h
#define KeyFrame_h

#include "RGB.h"

/////////////////////////////////////////////////////////////////////
/// \class KeyFrame
/// \brief Target Key Frame consisting of a target time and a target value
/////////////////////////////////////////////////////////////////////

class KeyFrame
{
  public:
    KeyFrame(int id, long time_ms, int target) 
     : _id(id), _time_ms(time_ms), _target(target){}
    KeyFrame(long time_ms, int target) 
     : _id(0), _time_ms(time_ms), _target(target){}
    KeyFrame() 
     : _id(0), _time_ms(0), _target(0){}
    
    long getTimeMs() {
      return _time_ms;
    }

    int getTarget() {
      return _target;
    }

    int getId() {
      return _id;
    }

  private:
    long _time_ms;
    int _target;
    int _id;
};

class KeyFrameRgb
{
  public:
    KeyFrameRgb(long time_ms, int red, int green, int blue, int brightness) 
      : _time_ms(time_ms), _color(red, green, blue, brightness){}
    KeyFrameRgb(long time_ms, int red, int green, int blue) 
      : _time_ms(time_ms), _color(red, green, blue) {}
    KeyFrameRgb() 
      : _color(0,0,0) {}
    
    long getTimeMs() {
      return _time_ms;
    }

    RGB getColor(){
        return _color;
    }

    int getRed() {
      return _color.red();
    }

    int getGreen() {
      return _color.green();
    }

    int getBlue() {
      return _color.blue();
    }

  private:
    long _time_ms;
    RGB _color;
};

#endif
