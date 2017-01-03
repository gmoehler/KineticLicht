#include "Helpers.h"

#define DEBUG false

void printRGB(int id, RGB col){
  serPrintln("LED%d, %d, %d, %d", id, col.red(), col.green(), col.blue());
}

void updateLEDs(Adafruit_TLC5947 tlc, int rgb1oId, RGB rgb1oColor, int rgb1uId, RGB rgb1uColor, int rgb4oId, RGB rgb4oColor, int rgb4uId, RGB rgb4uColor){

    //Serial.println ("***Update LEDs");
    //printRGB(rgb1oId, rgb1oColor);
    tlc.setLED(rgb1oId, rgb1oColor.red(), rgb1oColor.green(), rgb1oColor.blue());
    //printRGB(rgb1uId, rgb1uColor);
    tlc.setLED(rgb1uId, rgb1uColor.red(), rgb1uColor.green(), rgb1uColor.blue());
    /*
    tlc.setLED(rgb2o.getId(), rgb2oColor.red(), rgb2oColor.green(), rgb2oColor.blue());
    tlc.setLED(rgb2u.getId(), rgb2uColor.red(), rgb2uColor.green(), rgb2uColor.blue());
    RGB rgb3oColor = rgb3o.getCurrentColor();
    tlc.setLED(rgb3o.getId(), rgb3oColor.red(), rgb3oColor.green(), rgb3oColor.blue());
    RGB rgb3uColor = rgbu3u.getCurrentColor();
    tlc.setLED(rgb3u.getId(), rgb3uColor.red(), rgb3uColor.green(), rgb3uColor.blue());
    */
    //printRGB(rgb4oId, rgb4oColor);
    tlc.setLED(rgb4oId, rgb4oColor.red(), rgb4oColor.green(), rgb4oColor.blue());
    //printRGB(rgb4uId, rgb4uColor);
    tlc.setLED(rgb4uId, rgb4uColor.red(), rgb4uColor.green(), rgb4uColor.blue());

    //tlc.setLED(3, rgb4uColor.red(), rgb4uColor.green(), rgb4uColor.blue());
    //tlc.setLED(4, rgb4uColor.red(), rgb4uColor.green(), rgb4uColor.blue());
    //tlc.setLED(5, rgb4uColor.red(), rgb4uColor.green(), rgb4uColor.blue());
    //tlc.setLED(6, rgb4uColor.red(), rgb4uColor.green(), rgb4uColor.blue());
    
    tlc.write();

}



void serPrint(char* const str, ...) {
  if (DEBUG) {
/*    int i, count = 0, j = 0, flag = 0;
    char temp[ARDBUFFER + 1];
    for (i = 0; str[i] != '\0'; i++)  if (str[i] == '%')  count++;
    va_list argv;
    va_start(argv, count);
    for (i = 0, j = 0; str[i] != '\0'; i++)
    {
      if (str[i] == '%')
      {
        temp[j] = '\0';
        Serial.print(temp);
        j = 0;
        temp[0] = '\0';
        switch (str[++i])
        {
          case 'd': Serial.print(va_arg(argv, int));
            break;
          case 'l': Serial.print(va_arg(argv, long));
            break;
          case 'f': Serial.print(va_arg(argv, double));
            break;
          case 'c': Serial.print((char)va_arg(argv, int));
            break;
          case 's': Serial.print(va_arg(argv, char *));
            break;
          default:  ;
        };
      }
      else
      {
        temp[j] = str[i];
        j = (j + 1) % ARDBUFFER;
        if (j == 0)
        {
          temp[ARDBUFFER] = '\0';
          Serial.print(temp);
          temp[0] = '\0';
        }
      }
    };
    Serial.println();
  */
    int i, j, count = 0;

    va_list argv;
    va_start(argv, str);
    for (i = 0, j = 0; str[i] != '\0'; i++) {
      if (str[i] == '%') {
        count++;

        Serial.write(reinterpret_cast<const uint8_t*>(str + j), i - j);

        switch (str[++i]) {
          case 'd': Serial.print(va_arg(argv, int));
            break;
          case 'l': Serial.print(va_arg(argv, long));
            break;
          case 'f': Serial.print(va_arg(argv, double));
            break;
          case 'c': Serial.print((char) va_arg(argv, int));
            break;
          case 's': Serial.print(va_arg(argv, char *));
            break;
          case '%': Serial.print("%");
            break;
          default:;
        };

        j = i + 1;
      }
    };
    va_end(argv);

    if (i > j) {
      Serial.write(reinterpret_cast<const uint8_t*>(str + j), i - j);
    }
}
}

void serPrintln(char* const str, ...) {
  if (DEBUG) {
/*    int i, count = 0, j = 0, flag = 0;
    char temp[ARDBUFFER + 1];
    for (i = 0; str[i] != '\0'; i++)  if (str[i] == '%')  count++;
    va_list argv;
    va_start(argv, count);
    for (i = 0, j = 0; str[i] != '\0'; i++)
    {
      if (str[i] == '%')
      {
        temp[j] = '\0';
        Serial.print(temp);
        j = 0;
        temp[0] = '\0';
        switch (str[++i])
        {
          case 'd': Serial.print(va_arg(argv, int));
            break;
          case 'l': Serial.print(va_arg(argv, long));
            break;
          case 'f': Serial.print(va_arg(argv, double));
            break;
          case 'c': Serial.print((char)va_arg(argv, int));
            break;
          case 's': Serial.print(va_arg(argv, char *));
            break;
          default:  ;
        };
      }
      else
      {
        temp[j] = str[i];
        j = (j + 1) % ARDBUFFER;
        if (j == 0)
        {
          temp[ARDBUFFER] = '\0';
          Serial.print(temp);
          temp[0] = '\0';
        }
      }
    };
    Serial.println();
  */
    int i, j, count = 0;

    va_list argv;
    va_start(argv, str);
    for (i = 0, j = 0; str[i] != '\0'; i++) {
      if (str[i] == '%') {
        count++;

        Serial.write(reinterpret_cast<const uint8_t*>(str + j), i - j);

        switch (str[++i]) {
          case 'd': Serial.print(va_arg(argv, int));
            break;
          case 'l': Serial.print(va_arg(argv, long));
            break;
          case 'f': Serial.print(va_arg(argv, double));
            break;
          case 'c': Serial.print((char) va_arg(argv, int));
            break;
          case 's': Serial.print(va_arg(argv, char *));
            break;
          case '%': Serial.print("%");
            break;
          default:;
        };

        j = i + 1;
      }
    };
    va_end(argv);

    if (i > j) {
      Serial.write(reinterpret_cast<const uint8_t*>(str + j), i - j);
    }
    Serial.println();
  
}
}

