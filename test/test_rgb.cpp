#include "test.h"

TEST(RGB_tests, red){
  RGB led = RGB(2000, 0 , 0);
  EXPECT_EQ(2000, led.red());
  led = RGB(2000, 0 , 0, 50);
  EXPECT_EQ(1000, led.red());
  led = RGB(-1000, 0 , 0);
  EXPECT_EQ(0, led.red());
  led = RGB(-1000, 0 , 0, 50);
  EXPECT_EQ(0, led.red());
  led = RGB(5000, 0 , 0);
  EXPECT_EQ(RGB_MAX_VAL, led.red());
  led = RGB(5000, 0 , 0, 50);
  EXPECT_EQ(2500, led.red());
  led = RGB(-1000, 0 , 0);
  EXPECT_EQ(0, led.red());
  led = RGB(-1000, 0 , 0, 50);
  EXPECT_EQ(0, led.red());
}

TEST(RGB_tests, green){
  RGB led = RGB(0, 2000 , 0);
  EXPECT_EQ(2000, led.green());
  led = RGB(0, 2000 , 0, 50);
  EXPECT_EQ(1000, led.green());
  led = RGB(0, -1000, 0);
  EXPECT_EQ(0, led.green());
  led = RGB(0, -1000, 0, 50);
  EXPECT_EQ(0, led.green());
  led = RGB(0, 5000 , 0);
  EXPECT_EQ(RGB_MAX_VAL, led.green());
  led = RGB(0, 5000, 0, 50);
  EXPECT_EQ(2500, led.green());
  led = RGB(0, -1000 , 0);
  EXPECT_EQ(0, led.green());
  led = RGB(0, -1000, 0, 50);
  EXPECT_EQ(0, led.green());
}

TEST(RGB_tests, blue){
  RGB led = RGB(0, 0, 2000);
  EXPECT_EQ(2000, led.blue());
  led = RGB(0, 0, 2000, 50);
  EXPECT_EQ(1000, led.blue());
  led = RGB(0, 0, -1000);
  EXPECT_EQ(0, led.blue());
  led = RGB(0, 0, -1000, 50);
  EXPECT_EQ(0, led.blue());
  led = RGB(0, 0, 5000);
  EXPECT_EQ(RGB_MAX_VAL, led.blue());
  led = RGB(0, 0, 5000, 50);
  EXPECT_EQ(2500, led.blue());
  led = RGB(0, 0, -1000);
  EXPECT_EQ(0, led.blue());
  led = RGB(0, 0, -1000, 50);
  EXPECT_EQ(0, led.blue());
}

TEST(RGB_tests, white){
  RGB led = RGB(2000, 2000, 2000);
  EXPECT_EQ(2000, led.red());
  EXPECT_EQ(2000, led.green());
  EXPECT_EQ(2000, led.blue());
  led = RGB(2000, 2000, 2000, 50);
  EXPECT_EQ(1000, led.red());
  EXPECT_EQ(1000, led.green());
  EXPECT_EQ(1000, led.blue());
  led = RGB(-1000, -1000, -1000);
  EXPECT_EQ(0, led.red());
  EXPECT_EQ(0, led.green());
  EXPECT_EQ(0, led.blue());
  led = RGB(-1000, -1000, -1000, 50);
  EXPECT_EQ(0, led.red());
  EXPECT_EQ(0, led.green());
  EXPECT_EQ(0, led.blue());
  led = RGB(5000, 5000, 5000);
  EXPECT_EQ(RGB_MAX_VAL, led.red());
  EXPECT_EQ(RGB_MAX_VAL, led.green());
  EXPECT_EQ(RGB_MAX_VAL, led.blue());
  led = RGB(5000, 5000, 5000, 50);
  EXPECT_EQ(2500, led.red());
  EXPECT_EQ(2500, led.green());
  EXPECT_EQ(2500, led.blue());
  led = RGB(-1000, -1000, -1000);
  EXPECT_EQ(0, led.red());
  EXPECT_EQ(0, led.green());
  EXPECT_EQ(0, led.blue());
  led = RGB(-1000, -1000, -1000, 50);
  EXPECT_EQ(0, led.red());
  EXPECT_EQ(0, led.green());
  EXPECT_EQ(0, led.blue());
}

TEST(RGB_tests, yellow){
  RGB led = RGB(YELLOW);
  EXPECT_EQ(RGB_MAX_VAL, led.red());
  EXPECT_EQ(RGB_MAX_VAL, led.green());
  EXPECT_EQ(0, led.blue());

  led = RGB(RGB::rgb_yellow());
  EXPECT_EQ(RGB_MAX_VAL, led.red());
  EXPECT_EQ(RGB_MAX_VAL, led.green());
  EXPECT_EQ(0, led.blue());
}
