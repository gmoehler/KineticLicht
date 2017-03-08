#ifndef RGB_H
#define RGB_H

#define RGB_MAX_VAL 4095
#define RGB_COMPRESSION 16
#define RGB_MAX_COMPRESSED 255

#ifdef WITHIN_UNITTEST
  #include <stdint.h> // for int8_t
  #include <stdio.h>
#endif

// we define the colors like this so it is easy to use them in
// brace-enclosed initializer lists
#define WHITE RGB_MAX_VAL,RGB_MAX_VAL,RGB_MAX_VAL
#define BLACK 0,0,0
#define RED RGB_MAX_VAL,0,0
#define GREEN 0,RGB_MAX_VAL,0
#define BLUE 0,0,RGB_MAX_VAL
#define YELLOW RGB_MAX_VAL,RGB_MAX_VAL,0
#define LILA RGB_MAX_VAL,0,RGB_MAX_VAL
#define CYAN 0,RGB_MAX_VAL,RGB_MAX_VAL
#define NOCOLOR 0, 0, 0, 0 // includes brightness

class RGB {

  public:
    RGB(int red, int green, int blue, int brightness);
    RGB(int red, int green, int blue);
    RGB();
    int red();
    int green();
    int blue();

    static RGB rgb_white() { return RGB(WHITE);}
    static RGB rgb_black() { return RGB(BLACK);}
    static RGB rgb_red() { return RGB(RED);}
    static RGB rgb_green() { return RGB(GREEN);}
    static RGB rgb_blue() { return RGB(BLUE);}
    static RGB rgb_yellow() { return RGB(YELLOW);}
    static RGB rgb_lila() { return RGB(LILA);}
    static RGB rgb_cyan() { return RGB(CYAN);}

  private:
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;

    void _checkInput(int r, int g, int b);
    uint8_t _delimitCompress (int val);
};

#endif
