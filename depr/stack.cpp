template <class T> class FiniteStateMachine {
public:
  FiniteStateMachine(int numberOfStates, int initialState, T& obj);

  typedef bool (T::*TransitionCondFunction)(void);
  void addTransition(int fromState, int toState,  TransitionCondFunction transitionFunction);

  void loop();

private:
  int _numStates;
  int _state;

  T& _obj;   // the object to apply the functions on
  map<int,map<int,TransitionCondFunction>> _transitionMap;
}

template<class T>
FiniteStateMachine<T>::FiniteStateMachine(int numberOfStates, int initialState, T& obj) :
_numStates(numberOfStates), _state(initialState), _obj(obj), _transitionMap() {
}

template<class T>
void FiniteStateMachine<T>::addTransition(int fromState, int toState,  TransitionCondFunction transitionFunction){
  _transitionMap[fromState][toState] = transitionFunction;
}

template<class T>
void FiniteStateMachine<T>::loop(){

  auto it = _transitionMap.find(_state);
  if (it != _transitionMap.end()){
    map<int,TransitionCondFunction> innerTransMap = it->second;
    for(typename map<int,TransitionCondFunction>::iterator iter = innerTransMap.begin(); iter != innerTransMap.end(); ++iter) {

      bool (T::*tf)(void)  = iter->second;
      // is condition for transition fullfilled?
      if ((_obj.*tf)()) {
        int toState =  iter->first;
        // ok, transit to next state
        _state = toState;
        break;
      }
    }
  }
  else {
    printf("Cannot find transition condition function for state %d\n", _state);
  }
}

enum MyState {STATE0, STATE1, STATE2, NUM_STATES};

class MyTransitionClass : public FiniteStateMachine<MyTransitionClass>{
public:
  MyTransitionClass(int numberOfStates, int initialState) :
    FiniteStateMachine(numberOfStates, initialState, *this){
    addTransition(STATE0, STATE1, t1);
  }
  bool t1(){return _x>0;}

private:
  int _x = 99;
};



TEST(FiniteStates_test, testWithInheritedClass){

  MyTransitionClass mc = MyTransitionClass(2,STATE0);
  EXPECT_EQ(STATE0, mc.getState());
  mc.loop();
  EXPECT_EQ(STATE1, mc.getState());
}
