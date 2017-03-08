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