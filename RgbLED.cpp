#include "RgbLED.h"

RgbLED::RgbLED(int redPin, int greenPin, int bluePin) {

  //Rotary LED pins
  ledRedPin = redPin;
  ledGreenPin = greenPin;
  ledBluePin = bluePin;

  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
}

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

  int red_inv   = 255 - (factor * red);
  int green_inv = 255 - (factor * green);
  int blue_inv  = 255 - (factor * blue);

  analogWrite(ledRedPin, red_inv);
  analogWrite(ledGreenPin, green_inv);
  analogWrite(ledBluePin, blue_inv);

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

