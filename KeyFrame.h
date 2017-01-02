#ifndef KeyFrame_h
#define KeyFrame_h

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
      : _time_ms(time_ms), _red(red), _green(green), _blue(blue), _brightness(brightness){}
    KeyFrameRgb(long time_ms, int red, int green, int blue) 
      : _time_ms(time_ms), _red(red), _green(green), _blue(blue), _brightness(100){}
    KeyFrameRgb() 
      : _time_ms(0), _red(0), _green(0), _blue(0), _brightness(0){}
    
    long getTimeMs() {
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
    int getBrightness() {
      return _brightness;
    }


  private:
    long _time_ms;
    int _red;
    int _green;
    int _blue;
    int _brightness;
};

#endif
