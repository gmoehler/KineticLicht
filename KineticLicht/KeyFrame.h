#ifndef KeyFrame_h
#define KeyFrame_h

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#else
  #include <stdlib.h>
  #include <stdio.h>
#endif

#include "RGB.h"

enum KeyFrameType {MOTORFRAME, LEDFRAME, NOFRAMETYPE};

enum Activators { STEPPER1,  // 0
                  STEPPER2,  // 1
                  STEPPER3,  // 2
                  STEPPER4,  // 3
                  LED1TOP,   // 4
                  LED1BOT,   // 5
                  LED2TOP,   // 6
                  LED2BOT,   // 7
                  LED3TOP,   // 8
                  LED3BOT,   // 9
                  LED4TOP,   // 10
                  LED4BOT    // 11
                };

/*string id2String(unsigned id){
  switch (id) {
    case 0:
    return "STEPPER1";
    case 1:
    return "STEPPER2";
    case 2:
    return "STEPPER3";
    case 3:
    return "STEPPER4";
    case 4:
    return "LED1TOP";
    case 5:
    return "LED1BOT";
    case 6:
    return "LED2TOP";
    case 7:
    return "LED2BOT";
    case 8:
    return "LED3TOP";
    case 9:
    return "LED3BOT";
    case 10:
    return "LED4TOP";
    case 11:
    return "LED4BOT";
  }
  return "Unknown ID";
}
*/

/////////////////////////////////////////////////////////////////////
/// \class KeyFrame
/// \brief Target Key Frame consisting of a target time and a target value
/////////////////////////////////////////////////////////////////////

class KeyFrame
{
  public:
    // legacy
    KeyFrame(long timeMs, int targetPosition)
     : _id(0), _timeMs(timeMs), _targetPosition(targetPosition), _type(MOTORFRAME){}

    // keyframe for motor movement
    KeyFrame(int id, long timeMs, int targetPosition)
     : _id(id), _timeMs(timeMs), _targetPosition(targetPosition), _targetColor(BLACK), _type(MOTORFRAME){}

    // keyframe for led colors
    KeyFrame(int id, long timeMs, RGB targetColor)
      :  _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(targetColor), _type(LEDFRAME) {}
    KeyFrame(int id, long timeMs, int red, int green, int blue, int brightness)
      : _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(red, green, blue, brightness), _type(LEDFRAME){}
    KeyFrame(int id, long timeMs, int red, int green, int blue)
      :  _id(id), _timeMs(timeMs), _targetPosition(0), _targetColor(red, green, blue), _type(LEDFRAME){}
    KeyFrame()
     : _id(0), _timeMs(0), _targetPosition(0), _targetColor(0,0,0),_type(NOFRAMETYPE) {}

    // constructor from unsigned array
    KeyFrame(unsigned v[8])
    : _id((int) v[0]), _timeMs(100*v[1]), _targetPosition(v[2]),
      _targetColor((int) v[3], (int) v[4], (int) v[5], (int) v[6]), _type(v[7]==MOTORFRAME ? MOTORFRAME : LEDFRAME){}

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


  private:
    int _id;
    long _timeMs;
    int _targetPosition;
    RGB _targetColor;
    KeyFrameType _type;
};

#endif
