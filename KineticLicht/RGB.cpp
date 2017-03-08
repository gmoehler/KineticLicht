#include "RGB.h"

RGB::RGB() :
_red(0), _green(0), _blue(0){
};

RGB::RGB(int red, int green, int blue) {
  _checkInput(red, green, blue);
};

RGB::RGB(int red, int green, int blue, int brightness) {

  int actual_red   = (long) red   * brightness / 100;
  int actual_green = (long) green * brightness / 100;
  int actual_blue  = (long) blue  * brightness / 100;

  _checkInput(actual_red, actual_green, actual_blue);
};

void RGB::_checkInput(int red, int green, int blue){
  _red = _delimitCompress(red);
  _green = _delimitCompress(green);
  _blue = _delimitCompress(blue);
}

uint8_t RGB::_delimitCompress(int val){
  int v  = (val < 0) ? 0 : val;
  v  = (v > RGB_MAX_VAL) ? RGB_MAX_VAL : v;
  return (uint8_t) (v / RGB_COMPRESSION);
}

int RGB::red(){
  if (_red == RGB_MAX_COMPRESSED){
    return RGB_MAX_VAL;
  }
  return ((int) _red) * RGB_COMPRESSION;
}

int RGB::green(){
  if (_green == RGB_MAX_COMPRESSED){
    return RGB_MAX_VAL;
  }
  return (int) _green * RGB_COMPRESSION;
}

int RGB::blue(){
  if (_blue == RGB_MAX_COMPRESSED){
    return RGB_MAX_VAL;
  }
  return (int) _blue * RGB_COMPRESSION;
}
