#ifndef FINIITE_STATES_H
#define FINIITE_STATES_H

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
  void addStateEntryAction(int state, StateActionFunction stateEntryAction);
  void addStateExitAction(int state, StateActionFunction stateExitAction);

  void loop();

  void triggerTransition(int fromState, int toState);
  int getState();

private:
  int _numStates;
  int _state;

  T& _obj;   // the object to apply the functions on
  std::map<int,std::map<int,TransitionCondFunction>> _transitionMap;
  std::map<int,StateActionFunction> _stateActionMap;
  std::map<int,StateActionFunction> _stateEntryActionMap;
  std::map<int,StateActionFunction> _stateExitActionMap;
  std::map<int,int> _transitionTriggerMap;

  bool _checkStateMachine();
  void transit(int toState);
};


template<class T>
FiniteStateMachine<T>::FiniteStateMachine(int numberOfStates, int initialState, T& obj) :
_numStates(numberOfStates), _state(initialState), _obj(obj), _transitionMap() , _transitionTriggerMap(){
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
void FiniteStateMachine<T>::addStateEntryAction(int state,  StateActionFunction stateEntryActionFunction){
  _stateEntryActionMap[state] = stateEntryActionFunction;
}

template<class T>
void FiniteStateMachine<T>::addStateExitAction(int state,  StateActionFunction stateExitActionFunction){
  _stateExitActionMap[state] = stateExitActionFunction;
}

template<class T>
void FiniteStateMachine<T>::triggerTransition(int fromState, int toState){
  _transitionTriggerMap[fromState] =toState;
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
    std::map<int,TransitionCondFunction> innerTransMap = it->second;
    for(typename std::map<int,TransitionCondFunction>::iterator iter = innerTransMap.begin(); iter != innerTransMap.end(); ++iter) {

      bool (T::*tf)(void)  = iter->second;
      // is transition function  fullfilled?
      if ((_obj.*tf)()) {
        int toState =  iter->first;
        transit(toState);
        break;
      }
    }
  }
  else {
    printf("Cannot find transition condition function for state %d\n", _state);
  }

  // work on current state action function
  auto iter = _stateActionMap.find(_state);
  if (iter != _stateActionMap.end()){
    void (T::*saf)(void)  = iter->second;
    (_obj.*saf)();
  }
  else {
    printf("No action function defined for state %d\n", _state);
  }
}

template<class T>
void FiniteStateMachine<T>::transit(int toState){
	// call exit function of last state
        auto it1 = _stateExitActionMap.find(_state);
        if (it1 != _stateExitActionMap.end()){
          void (T::*sexitaf)(void)  = it1->second;
          (_obj.*sexitaf)();
        }
        // transition to next state
        _state = toState;
        // call entry function of new state
        auto it2 = _stateEntryActionMap.find(_state);
        if (it2 != _stateEntryActionMap.end()){
          void (T::*sentryaf)(void)  = it2->second;
          (_obj.*sentryaf)();
        }
	
	}

#endif
