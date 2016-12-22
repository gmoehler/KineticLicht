#ifndef RGB_H
#define RGB_H

class RGB {

  public:
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


