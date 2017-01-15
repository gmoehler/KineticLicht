#include "../RGB.h"
#include "stdio.h"
#include "gtest/gtest.h"


/*int main(int argc, char **argv){

  RGB led = RGB(5000, 0 , 0, 50);
  printf("Hello word in %d.\n", led.red());
}
*/

TEST(RGB_tests, red){
  RGB led1 = RGB(5000, 0 , 0);
  EXPECT_EQ(RGB_MAX_VAL, led1.red());
  RGB led2 = RGB(5000, 0 , 0, 50);
  EXPECT_EQ(2500, led2.red());
}
