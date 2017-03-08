#include "test.h"

TEST(RGB_tests, red){
  RGB led = RGB(2000, 0 , 0);
  RGB_EXPECT_EQ(2000, led.red());
  led = RGB(2000, 0 , 0, 50);
  RGB_EXPECT_EQ(1000, led.red());
  led = RGB(5000, 0 , 0);
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.red());
  led = RGB(5000, 0 , 0, 50);
  RGB_EXPECT_EQ(2500, led.red());

}

TEST(RGB_tests, green){
  RGB led = RGB(0, 2000 , 0);
  RGB_EXPECT_EQ(2000, led.green());
  led = RGB(0, 2000 , 0, 50);
  RGB_EXPECT_EQ(1000, led.green());
  led = RGB(0, 5000 , 0);
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.green());
  led = RGB(0, 5000, 0, 50);
  RGB_EXPECT_EQ(2500, led.green());
}

TEST(RGB_tests, blue){
  RGB led = RGB(0, 0, 2000);
  RGB_EXPECT_EQ(2000, led.blue());
  led = RGB(0, 0, 2000, 50);
  RGB_EXPECT_EQ(1000, led.blue());
  led = RGB(0, 0, 5000);
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.blue());
  led = RGB(0, 0, 5000, 50);
  RGB_EXPECT_EQ(2500, led.blue());
}

TEST(RGB_tests, white){
  RGB led = RGB(2000, 2000, 2000);
  RGB_EXPECT_EQ(2000, led.red());
  RGB_EXPECT_EQ(2000, led.green());
  RGB_EXPECT_EQ(2000, led.blue());
  led = RGB(2000, 2000, 2000, 50);
  RGB_EXPECT_EQ(1000, led.red());
  RGB_EXPECT_EQ(1000, led.green());
  RGB_EXPECT_EQ(1000, led.blue());

  led = RGB(5000, 5000, 5000);
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.red());
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.green());
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.blue());
  led = RGB(5000, 5000, 5000, 50);
  RGB_EXPECT_EQ(2500, led.red());
  RGB_EXPECT_EQ(2500, led.green());
  RGB_EXPECT_EQ(2500, led.blue());

}

TEST(RGB_tests, yellow){
  RGB led = RGB(YELLOW);
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.red());
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.green());
  RGB_EXPECT_EQ(0, led.blue());

  led = RGB(RGB::rgb_yellow());
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.red());
  RGB_EXPECT_EQ(RGB_MAX_VAL, led.green());
  RGB_EXPECT_EQ(0, led.blue());
}
