#include "../test/test.h"

void test1(){

	unsigned t1[][7] = {
		{LED1BOT, 0, 0, BLACK, 0},
	};

	unsigned t2[][7] = {
		{LED1BOT, 0, 0, BLACK, 0},
		{LED1TOP, 0, 0, BLACK, 0},
	};

	unsigned t3[][7] = {
		{LED1BOT, 0, 0, BLACK, 0},
		{LED1TOP, 0, 0, BLACK, 0},
		{LED2BOT, 0, 0, BLACK, 0},
	};

	unsigned ***allt = 0;
	allt = new unsigned**[3];

	int rows[3] = {1,2,3};

	//int rows1 = sizeof(t1) / sizeof(t1[0][0]);
  auto t_heap1 = new unsigned[rows[0]][7]();
	std::copy(&t1[0][0], &t1[0][0]+rows[0]*7,&t_heap1[0][0]);
	allt[0] = (unsigned**) t_heap1;

	//int rows2 = sizeof(t2) / sizeof(t2[0][0]);
  auto t_heap2 = new unsigned[rows[1]][7]();
	std::copy(&t2[0][0], &t2[0][0]+rows[1]*7,&t_heap2[0][0]);
	allt[1] = (unsigned**) t_heap2;

	//int rows3 = sizeof(t3) / sizeof(t3[0][0]);
	auto t_heap3 = new unsigned[rows[2]][7]();
	std::copy(&t3[0][0], &t3[0][0]+rows[2]*7,&t_heap3[0][0]);
	allt[2] = (unsigned**) t_heap3;

	for (int i=0; i<3; i++){
		for (int j=0; j<rows[i]; j++){
		 for (int k=0; k<7; k++){
			 printf("%d", allt[i][j][k]);
		 }
		 printf ("\n");
		}
	}

}


int main( int argc, const char* argv[] ){
	test1();

}
