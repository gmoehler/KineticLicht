#include "test.h"

enum MyState {STATE0, STATE1, STATE2, NUM_STATES};

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

  FiniteStateMachine fsm = FiniteStateMachine(NUM_STATES, INIT);

  fsm.addTransition(STATE0, STATE1, trans_init_state1);
  fsm.addTransition(STATE1, STATE2, trans_state1_state2);
  fsm.addStateAction(STATE0, action_init);
  fsm.addStateAction(STATE1, action_state1);
  fsm.addStateAction(STATE2, action_state2);

  EXPECT_EQ(STATE0, fsm.getState());
  fsm.loop();
  EXPECT_EQ(STATE1, fsm.getState());
  fsm.loop();
  EXPECT_EQ(STATE2, fsm.getState());
  fsm.loop();
  EXPECT_EQ(STATE2, fsm.getState());
}

class FSM_Wrapper: public FiniteStateMachine{
public:
  FSM_Wrapper(int numberOfStates, int initialState) :
    FiniteStateMachine(numberOfStates, initialState){
      addTransition(STATE0, STATE1, t1);
      addStateAction(a);
  }

  bool t1(){return true;}
  void a(){printf("action in state1");}

};

TEST(FiniteStates_test, testWitinClass){

  FSM_Wrapper fsmw = FSM_Wrapper(2,STATE0);


}
