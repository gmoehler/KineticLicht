#ifndef HELPERS_H
#define HELPERS_H

#include "RGB.h"
#include "Adafruit_TLC5947.h"
#include "KeyFrameRgbLED.h"

void serPrintln(char* const str, ...);
void serPrint(char* const str, ...);

void updateLEDs(Adafruit_TLC5947 tlc, int rgb1oId, RGB rgb1oColor, int rgb1uId, RGB rgb1uColor, int rgb4oId, RGB rgb4oColor, int rgb4uId, RGB rgb4uColor);

#endif
