#ifndef RgbLED_h
#define RgbLED_h

#include "Arduino.h"
#include "Adafruit_TLC5947.h"

#define COLOR_MAX_VAL 4095

#define WHITE COLOR_MAX_VAL,COLOR_MAX_VAL,COLOR_MAX_VAL
#define BLACK 0,0,0
#define RED COLOR_MAX_VAL,0,0
#define GREEN 0,COLOR_MAX_VAL,0
#define BLUE 0,0,COLOR_MAX_VAL
#define YELLOW COLOR_MAX_VAL,COLOR_MAX_VAL,0
#define LILA COLOR_MAX_VAL,0,COLOR_MAX_VAL
#define CYAN 0,COLOR_MAX_VAL,COLOR_MAX_VAL

class RgbLED {

  public:
    RgbLED();
    RgbLED(Adafruit_TLC5947* tcl, int rgbId);

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
    Adafruit_TLC5947* _tlc;
    int _rgbId;
    bool _printFineSerialOut;
};



#endif
