#ifndef Animation1Frames_h
#define Animation1Frames_h

// full height: 4200 = 2100mm

KeyFrame motor1_kf[] {
  {0, 0},
  {2000, 0},
  {9000, 3000},
 {12000, 3000},
 {26000, 0},
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
KeyFrame motor4_kf[] {
  {3000, 1000},
  {5000, 1000}
};


KeyFrameRgb rgb1o_kf[] {
  {0,    YELLOW}, //0
  {1500, YELLOW}, //50
  {2000, YELLOW}, //50
  {9000, RED}, //100
  {9500, RED}, //100
 {11000, BLUE}, //100
 {12000, BLUE}, //100
 {26000, GREEN}, //50
 {27000, GREEN}, //50
 {28000, YELLOW}, //50
 {33000, YELLOW}, //0
};

KeyFrameRgb rgb1u_kf[] {
  {0,    YELLOW}, //0
  {1500, YELLOW}, //50
  {2000, YELLOW}, //50
  {9000, RED}, //100
  {9500, RED}, //100
 {11000, BLUE}, //100
 {12000, BLUE}, //100
 {26000, GREEN}, //50
 {27000, GREEN}, //50
 {28000, YELLOW}, //50
 {33000, YELLOW}, //0
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
KeyFrameRgb rgb4o_kf[] {
  {1000, RED},
  {2000, YELLOW},
  {5000, BLUE}
};
KeyFrameRgb rgb4u_kf[] {
  {1000, RED},
  {2000, YELLOW},
  {5000, BLUE}
};

#endif
