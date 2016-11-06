#ifndef RgbLED_h
#define RgbLED_h

#include "Arduino.h"

#define WHITE 255,255,255
#define BLACK 0,0,0
#define RED 255,0,0
#define GREEN 0,255,0
#define BLUE 0,0,255
#define YELLOW 255,255,0
#define LILA 255,0,255
#define CYAN 0,255,255

class RgbLED {

  public:
    RgbLED();
    RgbLED(int redPin, int greenPin, int bluePin);

    void white(int brightness = 100);
    void black(int brightness = 100);
    void red(int brightness = 100);
    void blue(int brightness = 100);
    void green(int brightness = 100);
    void yellow(int brightness = 100);
    void lila(int brightness = 100);
    void cyan(int brightness = 100);

    // rgb-Wert auf RGB LED  ausgeben
    void rgbOutput(int red, int green, int blue, int brightness = 100);

    // debug output to serial out
    void printToSerialRgb(int red, int green, int blue, int brightness);

    // do fine-grained debug output
    void doFineSerialOutput(bool doOutput);

  private:
    // output pins for rotary RGB
    int ledRedPin;
    int ledGreenPin;
    int ledBluePin;

    bool printFineSerialOut;
};



#endif
