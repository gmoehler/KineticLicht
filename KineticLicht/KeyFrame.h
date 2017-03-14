#ifndef KeyFrame_h
#define KeyFrame_h

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
  #include <Flash.h>
#else
  #include <stdlib.h>
  #include <stdio.h>
#endif

#include <RGB.h>
#include <string>

#define STEPPER_COMPRESSION 16

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
  LED4BOT,   // 11
  NO_ACTIVATOR
};

/////////////////////////////////////////////////////////////////////
/// \class KeyFrame
/// \brief Target Key Frame consisting of a target time and a target value
/////////////////////////////////////////////////////////////////////

class KeyFrame
{
public:
  // legacy
  KeyFrame(long timeMs, int targetPosition)
  : _id(0), _timeDs(timeMs/100), _targetPosition(targetPosition){}

  // keyframe for motor movement
  KeyFrame(int id, long timeMs, int targetPosition)
  : _id(id), _timeDs(timeMs/100), _targetPosition(targetPosition), _targetColor(BLACK){}

  // keyframe for led colors
  KeyFrame(int id, long timeMs, RGB targetColor)
  :  _id(id), _timeDs(timeMs/100), _targetPosition(0), _targetColor(targetColor) {}
  KeyFrame(int id, long timeMs, int red, int green, int blue, int brightness)
  : _id(id), _timeDs(timeMs/100), _targetPosition(0), _targetColor(red, green, blue, brightness){}
  KeyFrame(int id, long timeMs, int red, int green, int blue)
  :  _id(id), _timeDs(timeMs/100), _targetPosition(0), _targetColor(red, green, blue){}
  KeyFrame()
  : _id(NO_ACTIVATOR), _timeDs(0), _targetPosition(0), _targetColor(0,0,0){}

  // constructor from unsigned array
  KeyFrame(unsigned v[7])
  : _id((int) v[0]), _timeDs(v[1]), _targetPosition(v[2]),
  _targetColor((int) v[3], (int) v[4], (int) v[5], (int) v[6]){}

#ifndef WITHIN_UNITTEST
  KeyFrame(_FLASH_ARRAY v)
  : _id((int) v[0]), _timeDs(v[1]), _targetPosition(v[2]),
  _targetColor((int) v[3], (int) v[4], (int) v[5], (int) v[6]){}
#endif

  int getId() {
    return (int) _id;
  }

  long getTimeMs() {
    return _timeDs * 100;
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
    printf("KeyFrame %d: %ld ms, tPos: %d, red: %d, green: %d, blue: %d\n", _id, 100*(long)_timeDs,
    _targetPosition, _targetColor.red(), _targetColor.green(), _targetColor.blue());
  }

  std::string id2String(unsigned id){
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


  KeyFrameType getType(){
    if (_id >= NO_ACTIVATOR) {
      return NOFRAMETYPE;
    }
    return _id < LED1TOP ? MOTORFRAME : LEDFRAME;
  }


private:
  uint8_t _id;
  unsigned _timeDs;
  int _targetPosition;
  RGB _targetColor;
};

#endif
