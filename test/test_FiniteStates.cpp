#include "test.h"

int myNumber = 10;

bool trans_init_state1(){
  return myNumber > 1;
}

bool trans_state1_state2(){
  return myNumber > 3;
}

void action_init(){
  printf("init\n");
}

void action_state1(){
  printf("state1\n");
}

void action_state2(){
  printf("state2\n");
}


TEST(FiniteStates_test, test1){

  enum MyState {INIT, STATE1, STATE2, NUM_STATES};

  FinisteStateMachine fsm = FinisteStateMachine(NUM_STATES, INIT);

  fsm.addTransition(INIT, STATE1, trans_init_state1);
  fsm.addTransition(STATE1, STATE2, trans_state1_state2);
  fsm.addStateAction(INIT, action_init);
  fsm.addStateAction(STATE1, action_state1);
  fsm.addStateAction(STATE2, action_state2);

  EXPECT_EQ(INIT, fsm.getState());
  fsm.loop();
  EXPECT_EQ(STATE1, fsm.getState());
  fsm.loop();
  EXPECT_EQ(STATE2, fsm.getState());
  fsm.loop();
  EXPECT_EQ(STATE2, fsm.getState());

}
