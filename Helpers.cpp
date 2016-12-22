#include "Arduino.h"

#define DEBUG true

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

