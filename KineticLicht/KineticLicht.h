#ifndef KineticLicht_h
#define KineticLicht_h

#define FLASH_PRINTF0(format) const static char msg0[] PROGMEM = "format"; printf(msg0);
#define FLASH_PRINTF0b(format) const static char msg0b[] PROGMEM = "format"; printf(msg0b);
#define FLASH_PRINTF0c(format) const static char msg0c[] PROGMEM = "format"; printf(msg0c);
#define FLASH_PRINTF0d(format) const static char msg0d[] PROGMEM = "format"; printf(msg0d);
#define FLASH_PRINTF0e(format) const static char msg0e[] PROGMEM = "format"; printf(msg0e);

#define FLASH_PRINTF1(format, a) const static char msg1[] PROGMEM = "format"; printf(msg1, a);
#define FLASH_PRINTF1b(format, a) const static char msg1b[] PROGMEM = "format"; printf(msg1b, a);
#define FLASH_PRINTF1c(format, a) const static char msg1c[] PROGMEM = "format"; printf(msg1c, a);

#define FLASH_PRINTF2(format, a, b) const static char msg2[] PROGMEM = "format"; printf(msg2, a, b);
#define FLASH_PRINTF3(format, a, b, c) const static char msg3[] PROGMEM = "format"; printf(msg3, a, b);
#define FLASH_PRINTF3b(format, a, b, c) const static char msg3b[] PROGMEM = "format"; printf(msg3b, a, b);

#endif
