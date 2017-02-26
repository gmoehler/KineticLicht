#include "../test/test.h"


typedef unsigned int animation_as_uint_t [8];

animation_as_uint_t* allV0[4];
int rows0 = 0;

animation_as_uint_t* getAnis0(int id){
  unsigned v[][8] = {
    {LED1BOT, 20, 0, RED, 100, 2},
    {LED1TOP, 20, 0, BLACK, 0, 2}
  };

  rows0 = sizeof(v) / sizeof(v[0]);
  auto v_heap = new unsigned[rows0][8]();
  std::copy(&v[0][0], &v[0][0]+rows0*8,&v_heap[0][0]);
  int rows01 = sizeof(v_heap) / sizeof(v_heap[0]);

  printf("SIZE %d %d\n", rows0, rows01);

  for (int i=0; i<rows0; i++){
    for (int j=0; j<8; j++){
      printf("%u ", v[i][j]);
    }
    printf("\n");
  }
  for (int i=0; i<rows0; i++){
    for (int j=0; j<8; j++){
      printf("%u ", v_heap[i][j]);
    }
    printf("\n");
  }

  allV0[0] = v_heap;

  for (int i=0; i<rows0; i++){
    for (int j=0; j<8; j++){
      printf("%u ", allV0[0][i][j]);
    }
    printf("\n");
  }

  return allV0[id];
}

void cleanupAnis0(){
  for (int i=0; i<4; i++){
    delete[] allV0[i];
  }
}


int main( int argc, const char* argv[] )
{
  animation_as_uint_t* as = getAnis0(0);

  Adafruit_TLC5947 tlc = Adafruit_TLC5947();
  AnimationOps ao(tlc);

  int num0 = ao.getNumAnimations();
  EXPECT_EQ(0, num0);


  for (int i=0; i<rows0; i++){
    for (int j=0; j<8; j++){
      printf("%u ",as[i][j]);
    }
    printf("\n");
  }

  cleanupAnis0();
}
