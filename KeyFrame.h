#ifndef KeyFrame_h
#define KeyFrame_h

/////////////////////////////////////////////////////////////////////
/// \class KeyFrame
/// \brief Target Key Frame consisting of a target time and a target value
/////////////////////////////////////////////////////////////////////

class KeyFrame
{
  public:
    KeyFrame(int time_ms, int target) 
     : _time_ms(time_ms), _target(target){}
    KeyFrame() 
     : _time_ms(0), _target(0){}
    
    unsigned int getTimeMs() {
      return _time_ms;
    }

    int getTarget() {
      return _target;
    }

  private:
    unsigned int _time_ms;
    int _target;
};

class KeyFrameRgb
{
  public:
    KeyFrameRgb(int time_ms, int red, int green, int blue) 
      : _time_ms(time_ms), _red(red), _green(green), _blue(blue){}
    KeyFrameRgb() 
      : _time_ms(0), _red(0), _green(0), _blue(0){}
    
    unsigned int getTimeMs() {
      return _time_ms;
    }

    int getRed() {
      return _red;
    }

    int getGreen() {
      return _green;
    }

    int getBlue() {
      return _blue;
    }

  private:
    unsigned int _time_ms;
    int _red;
    int _green;
    int _blue;
};

#endif
