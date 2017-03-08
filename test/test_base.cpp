#include "test.h"

void RGB_EXPECT_EQ (int a, int b){

	if (a == RGB_MAX_VAL){
		EXPECT_EQ(a,b);
	}
	else {
		int a0 = a / RGB_COMPRESSION * RGB_COMPRESSION;
		EXPECT_EQ(a0, b);
	}

}

void expect_rgb(RGB rgb, int r, int g, int b){
  RGB_EXPECT_EQ(r, rgb.red());
  RGB_EXPECT_EQ(g, rgb.green());
  RGB_EXPECT_EQ(b, rgb.blue());
}
