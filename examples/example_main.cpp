#include "../test/test.h"

void addAnimation(int ***ary, int t [][5], int rows, int i){

  ary[i] = new int*[rows];
  for(int j= 0; j < rows; ++j) {
    ary[i][j] = new int[5];
    for(int k=0; k< 5; ++k) {
        ary[i][j][k] = t[j][k];
        }
    }
}

void test1(){

	int t1[][5] = {
		{0, 0, 0, 0, 0},
	};

	int t2[][5] = {
		{0, 0, 0, 0, 0},
		{1, 1, 1 ,1 , 1},
	};

	int t3[][5] = {
		{0, 0, 0, 0, 0},
		{1, 1, 1 ,1, 1},
		{2, 2, 2, 2, 2},
	};

int rows[3] = {1,2,3};

int sizeZ = 3;

int ***ary = new int**[sizeZ];

addAnimation(ary, t1,rows[0], 0);
addAnimation(ary, t2,rows[1], 1);
addAnimation(ary, t3,rows[2], 2);
/*

int ***ary = new int**[sizeZ];
for(int i= 0;  i< sizeZ; ++i) {
  ary[i] = new int*[rows[i]];
  for(int j= 0; j < rows[i]; ++j) {
    ary[i][j] = new int[sizeX];
    for(int k=0; k< sizeX; ++k) {
    	ary[i][j][k] = 99;
        switch (i){
        	case 0:
              ary[i][j][k] = t1[j][k];
              break;
            case 1:
              ary[i][j][k] = t2[j][k];
              break;
            case 2:
              ary[i][j][k] = t3[j][k];
              break;
        }
    }
  }
}
*/

printf("hallo\n");
	for (int i=0; i<3; i++){
		for (int j=0; j<rows[i]; j++){
		 for (int k=0; k<5; k++){
			 printf("%d", ary[i][j][k]);
		 }
		 printf ("\n");
		}
	}
/*

	//int rows1 = sizeof(t1) / sizeof(t1[0][0]);
    auto t_heap1= new int [rows[0]][5]();
	std::copy(&t1[0][0], &t1[0][0]+rows[0]*5,&t_heap1[0][0]);
	allt[0] = (int**) t_heap1;

	//int rows2 = sizeof(t2) / sizeof(t2[0][0]);
  auto t_heap2 = new int[rows[1]][5]();
	std::copy(&t2[0][0], &t2[0][0]+rows[1]*5,&t_heap2[0][0]);
	allt[1] = (int**) t_heap2;

	//int rows3 = sizeof(t3) / sizeof(t3[0][0]);
	auto t_heap3 = new int[rows[2]][5]();
	std::copy(&t3[0][0], &t3[0][0]+rows[2]*5,&t_heap3[0][0]);
	allt[2] = (int**) t_heap3;

    printf("hallo\n");
	for (int i=0; i<3; i++){
		for (int j=0; j<rows[i]; j++){
		 for (int k=0; k<5; k++){
			 printf("%d", allt[i][j][k]);
		 }
		 printf ("\n");
		}
	}
*/
}


int main( int argc, const char* argv[] ){
	test1();

}
