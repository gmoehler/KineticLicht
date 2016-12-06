#include "RgbLED.h"

RgbLED::RgbLED(Adafruit_TLC5947* tlc, int rgbId) :
  _rgbId(rgbId), _tlc(tlc){
    
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

  double factor = brightness / 100.0;

  int red_dimmed   = COLOR_MAX_VAL - (factor * red);
  int green_dimmed = COLOR_MAX_VAL - (factor * green);
  int blue_dimmed  = COLOR_MAX_VAL - (factor * blue);

  _tlc->setLED(_rgbId, red_dimmed, green_dimmed, blue_dimmed);
  printToSerialRgb(red, green, blue, brightness);
}

void RgbLED::doFineSerialOutput(bool doOutput) {
  printFineSerialOut = doOutput;
}

void RgbLED::printToSerialRgb(int red, int green, int blue, int bright) {
  if (printFineSerialOut) {
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

