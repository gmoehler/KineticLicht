#ifndef RgbLED_h
#define RgbLED_h

#include "Arduino.h" 
#include "RGB.h"

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
    RgbLED(int rgbId);

    RGB white(int brightness = 100);
    RGB black(int brightness = 100);
    RGB red(int brightness = 100);
    RGB blue(int brightness = 100);
    RGB green(int brightness = 100);
    RGB yellow(int brightness = 100);
    RGB lila(int brightness = 100);
    RGB cyan(int brightness = 100);

    // rgb-Wert auf RGB LED  ausgeben
    RGB getRGB(int red, int green, int blue, int brightness = 100);

    // debug output to serial out
    void printToSerialRgb(int red, int green, int blue, int brightness);

    // do fine-grained debug output
    void doFineSerialOutput(bool doOutput);

    int getId();

  private:
    int _rgbId;
    bool _printFineSerialOut;
};



#endif
