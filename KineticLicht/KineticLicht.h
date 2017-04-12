#ifndef KineticLicht_h
#define KineticLicht_h

#define PBUFFER_SIZE 100
extern char pbuffer[PBUFFER_SIZE];

#ifdef DEBUG_PRINT
#ifdef WITH_PROGMEM

#define FPRINTF0(msg_name, format) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer);
#define FPRINTF1(msg_name, format, a) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a);
#define FPRINTF2(msg_name, format, a, b) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b);
#define FPRINTF3(msg_name, format, a, b, c) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c);
#define FPRINTF4(msg_name, format, a, b, c, d) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c, d);
#define FPRINTF5(msg_name, format, a, b, c, d, e) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c, d, e);
#define FPRINTF6(msg_name, format, a, b, c, d, e, f) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c, d, e, f);
#define FPRINTF7(msg_name, format, a, b, c, d, e, f, g) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c, d, e, f, g);
#define FPRINTF8(msg_name, format, a, b, c, d, e, f, g, h) FLASH_STRING(msg_name, format); msg_name.copy(pbuffer, PBUFFER_SIZE); printf(pbuffer, a, b, c, d, e, f, g, h);

#else // not WITH_PROGMEM

#define FPRINTF0(msg_name, format) printf(format);
#define FPRINTF1(msg_name, format, a) printf(format, a);
#define FPRINTF2(msg_name, format, a, b) printf(format, a, b);
#define FPRINTF3(msg_name, format, a, b, c) printf(format, a, b, c);
#define FPRINTF4(msg_name, format, a, b, c, d) printf(format, a, b, c, d);
#define FPRINTF5(msg_name, format, a, b, c, d, e) printf(format, a, b, c, d, e);
#define FPRINTF6(msg_name, format, a, b, c, d, e, f) printf(format, a, b, c, d, e, f);
#define FPRINTF7(msg_name, format, a, b, c, d, e, f, g) printf(format, a, b, c, d, e, f, g);
#define FPRINTF8(msg_name, format, a, b, c, d, e, f, g, h) printf(format, a, b, c, d, e, f, g, h);

#endif // WITH_PROGMEM

#else //DEBUG_PRINT

// no debug print
#define FPRINTF0(msg_name, format)
#define FPRINTF1(msg_name, format, a)
#define FPRINTF2(msg_name, format, a, b)
#define FPRINTF3(msg_name, format, a, b, c)
#define FPRINTF4(msg_name, format, a, b, c, d)
#define FPRINTF5(msg_name, format, a, b, c, d, e)
#define FPRINTF6(msg_name, format, a, b, c, d, e, f)
#define FPRINTF7(msg_name, format, a, b, c, d, e, f, g)
#define FPRINTF8(msg_name, format, a, b, c, d, e, f, g, h)

#endif //DEBUG_PRINT

#endif
