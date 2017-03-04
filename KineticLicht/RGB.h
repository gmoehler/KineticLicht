#ifndef RGB_H
#define RGB_H

#define RGB_MAX_VAL 4095

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
    RGB(unsigned red, unsigned green, unsigned blue, unsigned brightness);
    RGB(unsigned red, unsigned green, unsigned blue);
    RGB();
    unsigned red();
    unsigned green();
    unsigned blue();

    static RGB rgb_white() { return RGB(WHITE);}
    static RGB rgb_black() { return RGB(BLACK);}
    static RGB rgb_red() { return RGB(RED);}
    static RGB rgb_green() { return RGB(GREEN);}
    static RGB rgb_blue() { return RGB(BLUE);}
    static RGB rgb_yellow() { return RGB(YELLOW);}
    static RGB rgb_lila() { return RGB(LILA);}
    static RGB rgb_cyan() { return RGB(CYAN);}

  private:
    unsigned _red;
    unsigned _green;
    unsigned _blue;

    void _checkInput();
};
// we define the colors like this so it is easy to use them in
// brace-enclosed initializer lists




#endif
