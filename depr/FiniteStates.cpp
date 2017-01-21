#include "FiniteStates.h"


FiniteStateMachine::FiniteStateMachine(int numberOfStates, int initialState) :
  _numStates(numberOfStates), _state(initialState), _transitionMap() {
}
void FiniteStateMachine::addTransition(int fromState, int toState,  TransFunc transitionFunction ){
  _transitionMap[fromState][toState] = transitionFunction;
}

void FiniteStateMachine::addStateAction(int state, StateActionFunc stateActionFunction){
  _stateActionMap[state] = stateActionFunction;
}

void FiniteStateMachine::setState(int state){
  _state = state;
}
int FiniteStateMachine::getState(){
  return _state;
}

void FiniteStateMachine::loop(){

  // check whether we have to move to new state
  // pick transition for current state
  map<int,TransFunc> innerTransMap = _transitionMap[_state];
  for(map<int,TransFunc>::iterator iter = innerTransMap.begin(); iter != innerTransMap.end(); ++iter) {

    TransFunc tf = iter->second;
    // is transition function  fullfilled?
    if ((tf)()) {
      int toState =  iter->first;
      // transition to next state
      _state = toState;
      break;
    }
  }

  // work on current state action function
  StateActionFunc saf = _stateActionMap[_state];
  (saf)();
}
