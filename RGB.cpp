
#include "RGB.h"

RGB::RGB(int red, int green, int blue) :
  _red(red), _green(green), _blue(blue){ 
    };

RGB::RGB() :
  _red(0), _green(0), _blue(0){ 
    };


int RGB::red(){
  return _red;
}

int RGB::green(){
  return _green;
}

int RGB::blue(){
  return _blue;
}

