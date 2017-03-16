#ifndef KineticLicht_h
#define KineticLicht_h

#ifdef WITH_PROGMEM

#define PBUFFER_SIZE 100
char pbuffer[PBUFFER_SIZE];


#define FPRINTF0(format, msg_name) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer);
#define FPRINTF1(format, msg_name, a) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a);
#define FPRINTF2(format, msg_name, a, b) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b);
#define FPRINTF3(format, msg_name, a, b, c) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c);
#define FPRINTF4(format, msg_name, a, b, c, d) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c, d);
#define FPRINTF5(format, msg_name, a, b, c, d, e) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c, d, e);
#define FPRINTF6(format, msg_name, a, b, c, d, e, f) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c, d, e, f);

#define FLASH_PRINTF0(format) const static char msg0[] PROGMEM = format; printf(msg0);
#define FLASH_PRINTF0b(format) const static char msg0b[] PROGMEM = format; printf(msg0b);
#define FLASH_PRINTF0c(format) const static char msg0c[] PROGMEM = format; printf(msg0c);
#define FLASH_PRINTF0d(format) const static char msg0d[] PROGMEM = format; printf(msg0d);
#define FLASH_PRINTF0e(format) const static char msg0e[] PROGMEM = format; printf(msg0e);

#define FLASH_PRINTF1(format, a) const static char msg1[] PROGMEM = format; printf(msg1, a);
#define FLASH_PRINTF1b(format, a) const static char msg1b[] PROGMEM = format; printf(msg1b, a);
#define FLASH_PRINTF1c(format, a) const static char msg1c[] PROGMEM = format; printf(msg1c, a);
#define FLASH_PRINTF1d(format, a) const static char msg1d[] PROGMEM = format; printf(msg1d, a);
#define FLASH_PRINTF1e(format, a) const static char msg1e[] PROGMEM = format; printf(msg1e, a);
#define FLASH_PRINTF1f(format, a) const static char msg1f[] PROGMEM = format; printf(msg1f, a);
#define FLASH_PRINTF1g(format, a) const static char msg1g[] PROGMEM = format; printf(msg1g, a);
#define FLASH_PRINTF1h(format, a) const static char msg1h[] PROGMEM = format; printf(msg1h, a);
#define FLASH_PRINTF1i(format, a) const static char msg1i[] PROGMEM = format; printf(msg1i, a);
#define FLASH_PRINTF1j(format, a) const static char msg1j[] PROGMEM = format; printf(msg1j, a);
#define FLASH_PRINTF1k(format, a) const static char msg1k[] PROGMEM = format; printf(msg1k, a);
#define FLASH_PRINTF1l(format, a) const static char msg1l[] PROGMEM = format; printf(msg1l, a);

#define FLASH_PRINTF2(format, a, b) const static char msg2[] PROGMEM = format; printf(msg2, a, b);

#define FLASH_PRINTF3(format, a, b, c) const static char msg3[] PROGMEM = format; printf(msg3, a, b, c);
#define FLASH_PRINTF3b(format, a, b, c) const static char msg3b[] PROGMEM = format; printf(msg3b, a, b, c);

#define FLASH_PRINTF4(format, a, b, c, d) const static char msg4[] PROGMEM = format; printf(msg4, a, b, c, d);

#define FLASH_PRINTF5(format, a, b, c, d, e) const static char msg5[] PROGMEM = format; printf(msg5, a, b, c, d, e);
#define FLASH_PRINTF5b(format, a, b, c, d, e) const static char msg5b[] PROGMEM = format; printf(msg5b, a, b, c, d, e);

#define FLASH_PRINTF6(format, a, b, c, d, e, f) const static char msg6[] PROGMEM = format; printf(msg6, a, b, c, d, e, f);

#else // WITH_PROGMEM

#define FPRINTF0(format, msg_name) printf(format);
#define FPRINTF1(format, msg_name, a) printf(format, a);
#define FPRINTF2(format, msg_name, a, b) printf(format, a, b);
#define FPRINTF3(format, msg_name, a, b, c) printf(format, a, b, c);
#define FPRINTF4(format, msg_name, a, b, c, d) printf(format, a, b, c, d);
#define FPRINTF5(format, msg_name, a, b, c, d, e) printf(format, a, b, c, d, e);
#define FPRINTF6(format, msg_name, a, b, c, d, e, f) printf(format, a, b, c, d, e, f);


#define FLASH_PRINTF0(format) printf(format);
#define FLASH_PRINTF0b(format) printf(format);
#define FLASH_PRINTF0c(format) printf(format);
#define FLASH_PRINTF0d(format) printf(format);
#define FLASH_PRINTF0e(format) printf(format);

#define FLASH_PRINTF1(format, a) printf(format, a);
#define FLASH_PRINTF1b(format, a) printf(format, a);
#define FLASH_PRINTF1c(format, a) printf(format, a);
#define FLASH_PRINTF1d(format, a) printf(format, a);
#define FLASH_PRINTF1e(format, a) printf(format, a);
#define FLASH_PRINTF1f(format, a) printf(format, a);
#define FLASH_PRINTF1g(format, a) printf(format, a);
#define FLASH_PRINTF1h(format, a) printf(format, a);
#define FLASH_PRINTF1i(format, a) printf(format, a);
#define FLASH_PRINTF1j(format, a) printf(format, a);
#define FLASH_PRINTF1k(format, a) printf(format, a);
#define FLASH_PRINTF1l(format, a) printf(format, a);

#define FLASH_PRINTF2(format, a, b) printf(format, a, b);

#define FLASH_PRINTF3(format, a, b, c) printf(format, a, b, c);
#define FLASH_PRINTF3b(format, a, b, c) printf(format, a, b, c);

#define FLASH_PRINTF4(format, a, b, c, d) printf(format, a, b, c, d);

#define FLASH_PRINTF5(format, a, b, c, d, e) printf(format, a, b, c, d, e);
#define FLASH_PRINTF5b(format, a, b, c, d, e) printf(format, a, b, c, d, e);

#define FLASH_PRINTF6(format, a, b, c, d, e, f) printf(format, a, b, c, d, e, f);


#endif // WITH_PROGMEM

#endif
