#include "RGB.h"

RGB::RGB() :
  _red(0), _green(0), _blue(0){
    };

RGB::RGB(int red, int green, int blue) :
  _red(red), _green(green), _blue(blue){
  _checkInput();
};

RGB::RGB(int red, int green, int blue, int brightness) {

  _red   = (long) red   * brightness / 100;
  _green = (long) green * brightness / 100;
  _blue  = (long) blue  * brightness / 100;

  _checkInput();
};

RGB::RGB(const RGB& rgb) :
  _red(rgb._red), _green(rgb._green), _blue(rgb._blue){
};

RGB::~RGB(){
  
}

void RGB::_checkInput(){
  _red   = (_red < 0) ? 0 : _red;
  _red   = (_red > RGB_MAX_VAL) ? RGB_MAX_VAL : _red;
  _green = (_green < 0) ? 0 : _green;
  _green = (_green > RGB_MAX_VAL) ? RGB_MAX_VAL : _green;
  _blue  = (_blue < 0) ? 0 : _blue;
  _blue  = (_blue > RGB_MAX_VAL) ? RGB_MAX_VAL : _blue;
}

int RGB::red(){
  return _red;
}

int RGB::green(){
  return _green;
}

int RGB::blue(){
  return _blue;
}
