#ifndef KeyFrame_h
#define KeyFrame_h

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#endif

#include "RGB.h"


/////////////////////////////////////////////////////////////////////
/// \class KeyFrame
/// \brief Target Key Frame consisting of a target time and a target value
/////////////////////////////////////////////////////////////////////

class KeyFrame
{
  public:
    // legacy
    KeyFrame(long timeMs, int targetPosition)
     : _id(0), _timeMs(timeMs), _targetPosition(targetPosition){}

    // keyframe for motor movement
    KeyFrame(int id, long timeMs, int targetPosition)
     : _id(id), _timeMs(timeMs), _targetPosition(targetPosition), _targetColor(BLACK){}

    // keyframe for led colors
    KeyFrame(int id, long timeMs, RGB targetColor)
      :  _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(targetColor) {}
    KeyFrame(int id, long timeMs, int red, int green, int blue, int brightness)
      : _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(red, green, blue, brightness){}
    KeyFrame(int id, long timeMs, int red, int green, int blue)
      :  _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(red, green, blue) {}
    KeyFrame()
     : _id(0), _timeMs(0), _targetPosition(0), _targetColor(0,0,0) {}

    int getId() {
      return _id;
    }

    long getTimeMs() {
      return _timeMs;
    }

    int getTarget() {
      return _targetPosition;
    }

    int getTargetPosition() {
      return _targetPosition;
    }

    RGB getTargetColor(){
        return _targetColor;
    }

    void printKeyFrame(){
      printf("KeyFrame %d: %ld ms, tPos: %d, red: %d, green: %d, blue: %d\n", _id, _timeMs,
            _targetPosition, _targetColor.red(), _targetColor.green(), _targetColor.blue());
    }

  private:
    int _id;
    long _timeMs;
    int _targetPosition;
    RGB _targetColor;

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
