/***************************************************
  This is a library for our Adafruit 24-channel PWM/LED driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1429

  These drivers uses SPI to communicate, 3 pins are required to
  interface: Data, Clock and Latch. The boards are chainable

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
//Use hardwired pins in order to use fast digital write
#define LED_DATA   4
#define LED_CLOCK  5
#define LED_LATCH  6


#include "Adafruit_TLC5947.h"

Adafruit_TLC5947::Adafruit_TLC5947(uint8_t n, uint8_t c, uint8_t d, uint8_t l) {
  numdrivers = n;
  _clk = c;
  _dat = d;
  _lat = l;

  pwmbuffer = (uint16_t *)calloc(2, 24 * n);
}

void Adafruit_TLC5947::write(void) {
  digitalWriteFast(LED_LATCH, LOW);
  // 24 channels per TLC5974
  for (int8_t c = 24 * numdrivers - 1; c >= 0 ; c--) {
    // 12 bits per channel, send MSB first
    for (int8_t b = 11; b >= 0; b--) {
      digitalWriteFast(LED_CLOCK, LOW);

      if (pwmbuffer[c] & (1 << b))  {
        digitalWriteFast(LED_DATA, HIGH);
      }
      else {
        digitalWriteFast(LED_DATA, LOW);
      }
      digitalWriteFast(LED_CLOCK, HIGH);
    }
  }
  digitalWriteFast(LED_CLOCK, LOW);

  digitalWriteFast(LED_LATCH, HIGH);
  digitalWriteFast(LED_LATCH, LOW);
}



void Adafruit_TLC5947::setPWM(uint8_t chan, uint16_t pwm) {
  if (pwm > 4095) pwm = 4095;
  if (chan > 24 * numdrivers) return;
  pwmbuffer[chan] = pwm;
}


void Adafruit_TLC5947::setLED(uint8_t lednum, uint16_t r, uint16_t g, uint16_t b) {
  setPWM(lednum * 3, r);
  setPWM(lednum * 3 + 1, g);
  setPWM(lednum * 3 + 2, b);
}


boolean Adafruit_TLC5947::begin() {
  if (!pwmbuffer) return false;

  pinModeFast(LED_DATA, OUTPUT);
  pinModeFast(LED_CLOCK, OUTPUT);
  pinModeFast(LED_LATCH, OUTPUT);

  digitalWriteFast(LED_LATCH, LOW);

  return true;
}
