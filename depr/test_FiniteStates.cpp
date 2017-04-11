#include "test.h"

enum MyState {STATE0, STATE1, STATE2, STATE3, STATE4};
/*
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
*/


class MyClass{
public:
  MyClass(int numberOfStates, int initialState) :
    _myFsm(numberOfStates, initialState, *this){
    _myFsm.addTransition(STATE0, STATE1, &MyClass::t1);
    _myFsm.addTransition(STATE1, STATE2, &MyClass::t2);
    _myFsm.addStateAction(STATE0,  &MyClass::s0);
    _myFsm.addStateAction(STATE1,  &MyClass::s1);
    _myFsm.addStateAction(STATE2,  &MyClass::s2);
  }

  bool t1(){return _x>0;}
  bool t2(){return _x>0;}
  void s0(){printf("action in state0: %d\n", _x);}
  void s1(){printf("action in state1: %d\n", _x);}
  void s2(){printf("action in state2: %d\n", _x);}

  int getState(){
    return _myFsm.getState();
  }
  void loop(){
    _myFsm.loop();
  }

private:
  FiniteStateMachine<MyClass> _myFsm;
  int _x = 99;

};

TEST(FiniteStates_test, testWithClass){

  MyClass mc = MyClass(3,STATE0);
  EXPECT_EQ(STATE0, mc.getState());
  mc.loop();
  EXPECT_EQ(STATE1, mc.getState());
  mc.loop();
  EXPECT_EQ(STATE2, mc.getState());
  mc.loop();
  EXPECT_EQ(STATE2, mc.getState());

}

class MyTransitionClass : public FiniteStateMachine<MyTransitionClass>{
public:
  MyTransitionClass(int numberOfStates, int initialState) :
    FiniteStateMachine(numberOfStates, initialState, *this){
    addTransition(STATE0, STATE1, &MyTransitionClass::t1);
    addTransition(STATE1, STATE2,&MyTransitionClass:: t2);
    addStateAction(STATE0, &MyTransitionClass::s0);
    addStateEntryAction(STATE0, &MyTransitionClass::s0_entry);
    addStateExitAction(STATE0, &MyTransitionClass::s0_exit);
    addStateAction(STATE1, &MyTransitionClass::s1);
    addStateEntryAction(STATE1, &MyTransitionClass::s1_entry);
    addStateExitAction(STATE1, &MyTransitionClass::s1_exit);
    addStateAction(STATE2,&MyTransitionClass:: s2);
    addStateEntryAction(STATE2, &MyTransitionClass::s2_entry);
    addStateExitAction(STATE2, &MyTransitionClass::s2_exit);
    addStateAction(STATE3,&MyTransitionClass:: s3);
    addStateEntryAction(STATE3, &MyTransitionClass::s3_entry);
    addStateExitAction(STATE3, &MyTransitionClass::s3_exit);
    addStateAction(STATE4,&MyTransitionClass:: s4);
    addStateEntryAction(STATE4, &MyTransitionClass::s4_entry);
    addStateExitAction(STATE4, &MyTransitionClass::s4_exit);
  }

  bool t1(){return _x>0;}
  bool t2(){return _x>0;}
  void s0(){printf("action in state0: %d\n", _x);}
  void s0_entry(){printf("entry to state0: %d\n", _x);}
  void s0_exit(){printf("exit from state0: %d\n", _x);}
  void s1(){printf("action in state1: %d\n", _x);}
  void s1_entry(){printf("entry to state1: %d\n", _x);}
  void s1_exit(){printf("exit from state1: %d\n", _x);}
  void s2(){printf("action in state2: %d\n", _x);}
  void s2_entry(){printf("entry to state2: %d\n", _x);}
  void s2_exit(){printf("exit from state2: %d\n", _x);}
  void s3(){printf("action in state3: %d\n", _x);}
  void s3_entry(){printf("entry to state3: %d\n", _x);}
  void s3_exit(){printf("exit from state3: %d\n", _x);}
  void s4(){printf("action in state4: %d\n", _x);}
  void s4_entry(){printf("entry to state4: %d\n", _x);}
  void s4_exit(){printf("exit from state4: %d\n", _x);}

private:
  int _x = 99;

};

TEST(FiniteStates_test, testWithInheritedClass){

  MyTransitionClass mc = MyTransitionClass(4,STATE0);
  EXPECT_EQ(STATE0, mc.getState());
  mc.loop();
  EXPECT_EQ(STATE1, mc.getState());
  mc.loop();
  EXPECT_EQ(STATE2, mc.getState());
  mc.loop();
  EXPECT_EQ(STATE2, mc.getState());
  mc.triggerTransition(STATE2, STATE3);
  mc.loop();
  EXPECT_EQ(STATE3, mc.getState());
  mc.triggerTransition(STATE4, STATE2);
  mc.loop();
  EXPECT_EQ(STATE3, mc.getState());
  mc.triggerTransition(STATE3, STATE4);
  mc.loop();
  EXPECT_EQ(STATE4, mc.getState());
  mc.loop();
  EXPECT_EQ(STATE2, mc.getState());
  mc.loop(); // prev state transition 2->3 should no longer work
  EXPECT_EQ(STATE2, mc.getState());


}
