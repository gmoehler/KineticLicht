#include "RgbLED.h"

RgbLED::RgbLED(Adafruit_TLC5947* tlc, int rgbId) :
  _rgbId(rgbId), _tlc(tlc), _printFineSerialOut(false){ 
    };

void RgbLED::white(int brightness) {
  rgbOutput(WHITE, brightness);
}
void RgbLED::black(int brightness) {
  rgbOutput(BLACK, brightness);
}
void RgbLED::red(int brightness) {
  rgbOutput(RED, brightness);
}
void RgbLED::blue(int brightness) {
  rgbOutput(BLUE, brightness);
}
void RgbLED::green(int brightness) {
  rgbOutput(GREEN, brightness);
}
void RgbLED::yellow(int brightness) {
  rgbOutput(YELLOW, brightness);
}
void RgbLED::lila(int brightness) {
  rgbOutput(LILA, brightness);
}
void RgbLED::cyan(int brightness) {
  rgbOutput(CYAN, brightness);
}

void RgbLED::rgbOutput(int red, int green, int blue, int brightness) {

  int red_dimmed   = (long) red   * brightness / 100;
  int green_dimmed = (long) green * brightness / 100;
  int blue_dimmed  = (long) blue  * brightness / 100;

  red_dimmed   = (red_dimmed < 0) ? 0 : red_dimmed;
  green_dimmed = (green_dimmed < 0) ? 0 : green_dimmed;
  blue_dimmed  = (blue_dimmed < 0) ? 0 : blue_dimmed;

  _tlc->setLED(_rgbId, red_dimmed, green_dimmed, blue_dimmed);
  _tlc->write();
  printToSerialRgb(red, green, blue, brightness);
  printToSerialRgb(red_dimmed, green_dimmed, blue_dimmed, brightness);
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

