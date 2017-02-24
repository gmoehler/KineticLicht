#include "RGB.h"

RGB::RGB() :
  _red(0), _green(0), _blue(0){
    };

RGB::RGB(unsigned red, unsigned green, unsigned blue) :
  _red(red), _green(green), _blue(blue){
  _checkInput();
};

RGB::RGB(unsigned red, unsigned green, unsigned blue, unsigned brightness) {

  _red   = (long) red   * brightness / 100;
  _green = (long) green * brightness / 100;
  _blue  = (long) blue  * brightness / 100;

  _checkInput();
};

void RGB::_checkInput(){
  _red   = (_red > RGB_MAX_VAL) ? RGB_MAX_VAL : _red;
  _green = (_green > RGB_MAX_VAL) ? RGB_MAX_VAL : _green;
  _blue  = (_blue > RGB_MAX_VAL) ? RGB_MAX_VAL : _blue;
}

unsigned RGB::red(){
  return _red;
}

unsigned RGB::green(){
  return _green;
}

unsigned RGB::blue(){
  return _blue;
}
