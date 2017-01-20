#include "FiniteStates.h"


FinisteStateMachine::FinisteStateMachine(int numberOfStates, int initialState) :
  _numStates(numberOfStates), _state(initialState), _transitionMap() {
}
void FinisteStateMachine::addTransition(int fromState, int toState,  TransFunc transitionFunction ){
  _transitionMap[fromState][toState] = transitionFunction;
}

void FinisteStateMachine::addStateAction(int state, StateActionFunc stateActionFunction){
  _stateActionMap[state] = stateActionFunction;
}

void FinisteStateMachine::setState(int state){
  _state = state;
}
int FinisteStateMachine::getState(){
  return _state;
}

void FinisteStateMachine::loop(){

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
