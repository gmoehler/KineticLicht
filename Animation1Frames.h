#ifndef Animation1Frames_h
#define Animation1Frames_h

// full height: 4200 = 2100mm

int motor1_num = 5;
KeyFrame motor1_kf[] {
  {0, 0},
  {2000, 0},
  {9000, 2600},
 {12000, 2600},
 {26000, 10},
};

/*
KeyFrame motor2_kf[] {
  {5000, 1000},
  {7000, 200}
};

KeyFrame motor3_kf[] {
  {5000, 2000},
  {7000, 1500}
};
*/

int motor4_num = 5;
KeyFrame motor4_kf[] {
  {0, 0},
  {3000, 0},
 {10000, 2800},
 {13000, 2800},
 {27000, 10},
};


int rgb1o_num = 11;
KeyFrameRgb rgb1o_kf[] {
  {0,    YELLOW, 0}, 
  {1500, YELLOW, 50}, 
  {2000, YELLOW, 50}, 
  {9000, RED}, //100
  {9500, RED}, //100
 {11000, BLUE}, //100
 {12000, BLUE}, //100
 {26000, GREEN, 50},
 {27000, GREEN, 50}, 
 {28000, YELLOW, 50},
 {33000, YELLOW, 0},
};

int rgb1u_num = 11;
KeyFrameRgb rgb1u_kf[] {
  {0,    YELLOW, 0},
  {3500, YELLOW, 50},
  {4000, YELLOW, 50},
  {11000, RED}, //100
  {11500, RED}, //100
 {13000, BLUE}, //100
 {14000, BLUE}, //100
 {28000, GREEN, 50}, 
 {29000, GREEN, 50},
 {30000, YELLOW, 50}, 
 {35000, YELLOW, 0}, 
};

/*
KeyFrameRgb rgb2o_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb2u_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb3o_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
KeyFrameRgb rgb3u_kf[] {
  {2000, RED},
  {4000, YELLOW},
  {7000, BLUE}
};
*/

int rgb4o_num = 12;
KeyFrameRgb rgb4o_kf[] {
  {0,    YELLOW, 0}, 
  {1000, YELLOW, 0}, 
  {2500, YELLOW, 50},
  {3000, YELLOW, 50}, 
 {11000, RED, 100}, //100
 {11500, RED, 100}, //100
 {13000, BLUE, 100}, //100
 {14000, BLUE, 100}, //100
 {28000, GREEN, 50}, 
 {29000, GREEN, 50}, 
 {30000, YELLOW, 50}, 
 {35000, YELLOW, 50},
};
int rgb4u_num = 12;
KeyFrameRgb rgb4u_kf[] {
  {0,    YELLOW, 0},
  {1000,    YELLOW, 0},
  {4500, YELLOW, 50}, 
  {5000, YELLOW, 50}, 
 {14000, RED}, //100
 {13500, RED}, //100
 {15000, BLUE}, //100
 {16000, BLUE}, //100
 {30000, GREEN, 50}, 
 {31000, GREEN, 50}, 
 {32000, YELLOW, 50},
 {37000, YELLOW, 0}, 
};

//animation 2
/*int motor1_num = 3;
KeyFrame motor1_kf[] {
  {0, 0},
  {4000, 3000},
  {8000, 0},
};

int motor4_num = 4;
KeyFrame motor4_kf[] {
  {0, 0},
  {2000, 0},
  {6000, 3000},
  {10000, 0},
};


int rgb1o_num = 7;
KeyFrameRgb rgb1o_kf[] {
  {400,  WHITE, 100}, 
  {1000, BLACK, 0}, 
  {2300,  BLACK, 0}, 
  {2400,  WHITE, 100}, 
  {3000, BLACK, 0}, 
  {20000, WHITE, 100}, 
  {22000, BLACK, 0}, 
};

int rgb1u_num = 7;
KeyFrameRgb rgb1u_kf[] {
  {200,  WHITE, 100}, 
  {800, BLACK, 0}, 
  {2000,  BLACK, 0}, 
  {2200,  WHITE, 100}, 
  {2800, BLACK, 0}, 
  {20000, WHITE, 100}, 
  {22000, BLACK, 0}, 
};

int rgb4o_num = 7;
KeyFrameRgb rgb4o_kf[] {
  {500,  WHITE, 100}, 
  {1000, BLACK, 0}, 
  {3600,  BLACK, 0}, 
  {3700,  WHITE, 100}, 
  {4100, BLACK, 0}, 
  {20000, WHITE, 100}, 
  {22000, BLACK, 0}, 
};
int rgb4u_num = 7;
KeyFrameRgb rgb4u_kf[] {
  {300,  WHITE, 100}, 
  {800, BLACK, 0}, 
  {3500,  BLACK, 0}, 
  {3600,  WHITE, 100}, 
  {4200, BLACK, 0}, 
  {20000, WHITE, 100}, 
  {22000, BLACK, 0}, 
};
*/
#endif
