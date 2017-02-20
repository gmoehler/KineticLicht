#ifndef KeyFrame_h
#define KeyFrame_h

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#endif

#include "RGB.h"

enum KeyFrameType {MOTOR, RGB_LED, NONE};


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
     : _id(id), _timeMs(timeMs), _targetPosition(targetPosition), _targetColor(BLACK), _type(MOTOR){}

    // keyframe for led colors
    KeyFrame(int id, long timeMs, RGB targetColor)
      :  _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(targetColor), _type(RGB_LED) {}
    KeyFrame(int id, long timeMs, int red, int green, int blue, int brightness)
      : _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(red, green, blue, brightness),_type(RGB_LED){}
    KeyFrame(int id, long timeMs, int red, int green, int blue)
      :  _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(red, green, blue), _type(RGB_LED){}
    KeyFrame()
     : _id(-1), _timeMs(0), _targetPosition(0), _targetColor(0,0,0),_type(NONE) {}

    // copy constructor
    KeyFrame (const KeyFrame &kf)
     : _id(kf._id), _timeMs(kf._timeMs), _targetPosition(kf._targetPosition),
       _targetColor(kf._targetColor), _type(kf._type){}

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

    ~KeyFrame(){}

  private:
    int _id;
    long _timeMs;
    int _targetPosition;
    RGB _targetColor;
    KeyFrameType _type;
};

#endif
