#ifndef FINIITE_STATES_H
#define FINIITE_STATES_H

#ifndef WITHIN_UNITTEST
#include <ArduinoSTL.h>
#else
#include "stdio.h"
#endif

#include <map>

//#define FSM_DEBUG

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

  // one time trigger from one state to another
  void triggerTransition(int fromState, int toState);
  int getState();

  void setDebug(bool debug);
  void setDebugString(const string s);

private:
  int _numStates;
  int _state;
  bool _debug;
  string _debugString;

  T& _obj;   // the object to apply the functions on
  std::map<int,std::map<int,TransitionCondFunction>> _transitionMap;
  std::map<int,StateActionFunction> _stateActionMap;
  std::map<int,StateActionFunction> _stateEntryActionMap;
  std::map<int,StateActionFunction> _stateExitActionMap;
  std::map<int,int> _transitionTriggerMap;

  void _transit(int toState);
};

template<class T>
void FiniteStateMachine<T>::setDebug(bool debug){
  _debug = debug;
}

template<class T>
void FiniteStateMachine<T>::setDebugString(const string debugString){
  _debugString = debugString;
}

template<class T>
FiniteStateMachine<T>::FiniteStateMachine(int numberOfStates, int initialState, T& obj) :
_numStates(numberOfStates), _state(initialState), _debug(true),
_obj(obj), _transitionMap(), _transitionTriggerMap(){
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
  _transitionTriggerMap[fromState] = toState;
}

template<class T>
int FiniteStateMachine<T>::getState(){
  return _state;
}

template<class T>
void FiniteStateMachine<T>::loop(){

  auto it1 = _transitionTriggerMap.find(_state);
  // transitionConfFunctions found
  if (it1 != _transitionTriggerMap.end()){
    int toState = it1->second;
    _transit(toState);
    _transitionTriggerMap.erase(it1);
  }
  else {
    // check whether we have to move to new state
    // pick transition for current state
    auto it2 = _transitionMap.find(_state);
    // transitionConfFunctions found
    if (it2 != _transitionMap.end()){
      std::map<int,TransitionCondFunction> innerTransMap = it2->second;
      for(typename std::map<int,TransitionCondFunction>::iterator it3 = innerTransMap.begin();
          it3 != innerTransMap.end(); ++it3) {

        bool (T::*tf)(void)  = it3->second;
        // is transition function  fullfilled?
        if ((_obj.*tf)()) {
          int toState =  it3->first;
          _transit(toState);
          break;
        }
      }
    }
  }

  // work on current state action function
  auto iter = _stateActionMap.find(_state);
  if (iter != _stateActionMap.end()){
#ifdef FSM_DEBUG
    printf("FSM<%s>: state action %d\n" , _debugString.c_str(), _state);
#endif
    void (T::*saf)(void)  = iter->second;
    (_obj.*saf)();
  }
}

template<class T>
void FiniteStateMachine<T>::_transit(int toState){
  // call exit function of last state
  auto it1 = _stateExitActionMap.find(_state);
  if (it1 != _stateExitActionMap.end()){
#ifdef FSM_DEBUG
    printf("FSM<%s>: state exit %d\n", _debugString.c_str(), _state);
#endif
    void (T::*sexitaf)(void)  = it1->second;
    (_obj.*sexitaf)();
  }

  // transition to next state
  _state = toState;

  // call entry function of new state
  auto it2 = _stateEntryActionMap.find(_state);
  if (it2 != _stateEntryActionMap.end()){
#ifdef FSM_DEBUG
        printf("FSM<%s>: state entry %d\n", _debugString.c_str(), _state);
#endif
    void (T::*sentryaf)(void)  = it2->second;
    (_obj.*sentryaf)();
  }
}

#endif
