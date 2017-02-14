#include "RgbLED.h"
#include "Helpers.h"

RgbLED::RgbLED(int rgbId) :
  _rgbId(rgbId),_printFineSerialOut(false){ 
    };

int RgbLED::getId() {
  return _rgbId;
}


RGB RgbLED::white(int brightness) {
  return getRGB(WHITE, brightness);
}
RGB RgbLED::black(int brightness) {
  return getRGB(BLACK, brightness);
}
RGB RgbLED::red(int brightness) {
  return getRGB(RED, brightness);
}
RGB RgbLED::blue(int brightness) {
  return getRGB(BLUE, brightness);
}
RGB RgbLED::green(int brightness) {
  return getRGB(GREEN, brightness);
}
RGB RgbLED::yellow(int brightness) {
  return getRGB(YELLOW, brightness);
}
RGB RgbLED::lila(int brightness) {
  return getRGB(LILA, brightness);
}
RGB RgbLED::cyan(int brightness) {
  return getRGB(CYAN, brightness);
}

RGB RgbLED::getRGB(int red, int green, int blue, int brightness) {

  int red_dimmed   = (long) red   * brightness / 100;
  int green_dimmed = (long) green * brightness / 100;
  int blue_dimmed  = (long) blue  * brightness / 100;

  red_dimmed   = (red_dimmed < 0) ? 0 : red_dimmed;
  red_dimmed   = (red_dimmed > COLOR_MAX_VAL) ? COLOR_MAX_VAL : red_dimmed;
  green_dimmed = (green_dimmed < 0) ? 0 : green_dimmed;
  green_dimmed = (green_dimmed > COLOR_MAX_VAL) ? COLOR_MAX_VAL : green_dimmed;
  blue_dimmed  = (blue_dimmed < 0) ? 0 : blue_dimmed;
  blue_dimmed  = (blue_dimmed > COLOR_MAX_VAL) ? COLOR_MAX_VAL : blue_dimmed;

  
  //_tlc->setLED(_rgbId, red_dimmed, green_dimmed, blue_dimmed);
  //_tlc->write();
  printToSerialRgb(red, green, blue, brightness);
  printToSerialRgb(red_dimmed, green_dimmed, blue_dimmed, brightness);

  RGB rgb = RGB(red_dimmed, green_dimmed, blue_dimmed);
  return rgb;
}

void RgbLED::doFineSerialOutput(bool doOutput) {
  _printFineSerialOut = doOutput;
}

void RgbLED::printToSerialRgb(int red, int green, int blue, int bright) {
  if (_printFineSerialOut) {
    Serial.print(" ");
    Serial.print(bright);
    Serial.print("%: ");
    Serial.print(red);
    Serial.print(" ");
    Serial.print(green);
    Serial.print(" ");
    Serial.println(blue);
  }
}

