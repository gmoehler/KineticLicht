#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#else
  #include "stdio.h"
#endif

#include <map>

using namespace std;

template <class T> class FiniteStateMachine {
public:
  FiniteStateMachine(int numberOfStates, int initialState, T& obj);

  typedef bool (T::*TransitionCondFunction)(void);
  void addTransition(int fromState, int toState,  TransitionCondFunction transitionFunction);
  typedef void (T::*StateActionFunction)(void);
  void addStateAction(int state, StateActionFunction stateAction);

  void loop();

  void setState(int state);
  int getState();

private:
  int _numStates;
  int _state;

  T& _obj;   // the object to apply the functions on
  map<int,map<int,TransitionCondFunction>> _transitionMap;
  map<int,StateActionFunction> _stateActionMap;

  bool _checkStateMachine();
};


template<class T>
FiniteStateMachine<T>::FiniteStateMachine(int numberOfStates, int initialState, T& obj) :
_numStates(numberOfStates), _state(initialState), _obj(obj), _transitionMap() {
}
template<class T>
void FiniteStateMachine<T>::addTransition(int fromState, int toState,  TransitionCondFunction transitionFunction){
  _transitionMap[fromState][toState] = transitionFunction;
}

template<class T>
void FiniteStateMachine<T>::addStateAction(int state,  StateActionFunction stateActionFunction){
  _stateActionMap[state] = stateActionFunction;
}

template<class T>
void FiniteStateMachine<T>::setState(int state){
  _state = state;
}
template<class T>
int FiniteStateMachine<T>::getState(){
  return _state;
}

template<class T>
bool FiniteStateMachine<T>::_checkStateMachine(){

  //TODO: do more extensive checking
  if ((int)_transitionMap.size() != _numStates-1 ||(int) _stateActionMap.size() != _numStates) {
    printf("The statemachine was not set up properly. Num states: %d, Num Transition Conditions: %d, Num State Actions: %d\n",
    _numStates, _transitionMap.size(), _stateActionMap.size());
    return false;
  }

  return true;
}

template<class T>
void FiniteStateMachine<T>::loop(){

  if (!_checkStateMachine()){
    return;
  }

  // check whether we have to move to new state
  // pick transition for current state
  auto it = _transitionMap.find(_state);
  // transitionConfFunctions found
  if (it != _transitionMap.end()){
    map<int,TransitionCondFunction> innerTransMap = it->second;
    for(typename map<int,TransitionCondFunction>::iterator iter = innerTransMap.begin(); iter != innerTransMap.end(); ++iter) {

      bool (T::*tf)(void)  = iter->second;
      // is transition function  fullfilled?
      if ((_obj.*tf)()) {
        int toState =  iter->first;
        // transition to next state
        _state = toState;
        break;
      }
    }
  }
  else {
    printf("Cannot find transition condition function for state %d\n", _state);
  }

  // work on current state action function
  void (T::*saf)(void) = _stateActionMap[_state];
  (_obj.*saf)();
}
