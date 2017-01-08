#ifndef RGB_H
#define RGB_H

#define RGB_MAX_VAL 4095

class RGB {

  public:
    RGB(int red, int green, int blue, int brightness);
    RGB(int red, int green, int blue);
    RGB();
    int red();
    int green();
    int blue();

  private:
    int _red;
    int _green;
    int _blue;
};



#endif


