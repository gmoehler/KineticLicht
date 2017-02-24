#ifndef KeyFrame_h
#define KeyFrame_h

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#endif

#include "RGB.h"

enum KeyFrameType {MOTOR, RGB_LED, NOFRAMETYPE};


/////////////////////////////////////////////////////////////////////
/// \class KeyFrame
/// \brief Target Key Frame consisting of a target time and a target value
/////////////////////////////////////////////////////////////////////

class KeyFrame
{
  public:
    // legacy
    KeyFrame(long timeMs, unsigned targetPosition)
     : _id(0), _timeMs(timeMs), _targetPosition(targetPosition), _type(MOTOR){}

    // keyframe for motor movement
    KeyFrame(unsigned id, long timeMs, unsigned targetPosition)
     : _id(id), _timeMs(timeMs), _targetPosition(targetPosition), _targetColor(BLACK), _type(MOTOR){}

    // keyframe for led colors
    KeyFrame(unsigned id, long timeMs, RGB targetColor)
      :  _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(targetColor), _type(RGB_LED) {}
    KeyFrame(unsigned id, long timeMs, unsigned red, unsigned green, unsigned blue, unsigned brightness)
      : _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(red, green, blue, brightness),_type(RGB_LED){}
    KeyFrame(unsigned id, long timeMs, unsigned red, unsigned green, unsigned blue)
      :  _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(red, green, blue), _type(RGB_LED){}
    KeyFrame()
     : _id(0), _timeMs(0), _targetPosition(0), _targetColor(0,0,0),_type(NOFRAMETYPE) {}
    KeyFrame(unsigned v[8])
    : _id(v[0]), _timeMs(100*v[1]), _targetPosition(v[2]),
      _targetColor(v[3], v[4], v[5], v[6]),_type(v[7]==1 ? MOTOR : RGB_LED){}

    unsigned getId() {
      return _id;
    }

    long getTimeMs() {
      return _timeMs;
    }

    unsigned getTarget() {
      return _targetPosition;
    }

    unsigned getTargetPosition() {
      return _targetPosition;
    }

    RGB& getTargetColor(){
        return _targetColor;
    }

    void printKeyFrame(){
      printf("KeyFrame %d: %ld ms, tPos: %d, red: %d, green: %d, blue: %d\n", _id, _timeMs,
            _targetPosition, _targetColor.red(), _targetColor.green(), _targetColor.blue());
    }

    bool getType(){
    	return _type;
    }


  private:
    unsigned _id;
    long _timeMs;
    unsigned _targetPosition;
    RGB _targetColor;
    KeyFrameType _type;
};

#endif
