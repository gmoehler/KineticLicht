#ifndef FINIITE_STATES_H
#define FINIITE_STATES_H

#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#else
  #include <stdio.h>
#endif

#include <map>
#include <string>

//#define FSM_DEBUG


/**
 * A class that represents a Finite State Machine
 *
 * Use as a base class of your FSM class and define methods
 *  - that trigger transitions
 *  - that are called at state entry
 *  - that are called within each loop while a state is active
 *  - that are called on a state exit
 **/
template <class T> class FiniteStateMachine {
public:
  FiniteStateMachine(uint8_t numberOfStates, uint8_t initialState, T& obj);

  typedef bool (T::*TransitionCondFunction)(void);
  void addTransition(uint8_t fromState, uint8_t toState,  TransitionCondFunction transitionFunction);
  typedef void (T::*StateActionFunction)(void);
  void addStateAction(uint8_t state, StateActionFunction stateAction);
  void addStateEntryAction(uint8_t state, StateActionFunction stateEntryAction);
  void addStateExitAction(uint8_t state, StateActionFunction stateExitAction);

  void loop();

  // one time trigger from one state to another
  void triggerTransition(uint8_t fromState, uint8_t toState);
  uint8_t getState();

  void setDebug(bool debug);
  void setDebugString(const std::string s);

private:
  uint8_t _numStates;
  uint8_t _state;
  bool _debug;
  std::string _debugString;

  T& _obj;   // the object to apply the functions on
  std::map<uint8_t,std::map<uint8_t,TransitionCondFunction>> _transitionMap;
  std::map<uint8_t,StateActionFunction> _stateActionMap;
  std::map<uint8_t,StateActionFunction> _stateEntryActionMap;
  std::map<uint8_t,StateActionFunction> _stateExitActionMap;
  std::map<uint8_t,uint8_t> _transitionTriggerMap;

  void _transit(uint8_t toState);
};

template<class T>
void FiniteStateMachine<T>::setDebug(bool debug){
  _debug = debug;
}

template<class T>
void FiniteStateMachine<T>::setDebugString(const std::string debugString){
  _debugString = debugString;
}

template<class T>
FiniteStateMachine<T>::FiniteStateMachine(uint8_t numberOfStates, uint8_t initialState, T& obj) :
_numStates(numberOfStates), _state(initialState), _debug(true),
_obj(obj), _transitionMap(), _transitionTriggerMap(){
}
template<class T>
void FiniteStateMachine<T>::addTransition(uint8_t fromState, uint8_t toState,  TransitionCondFunction transitionFunction){
  _transitionMap[fromState][toState] = transitionFunction;
}

template<class T>
void FiniteStateMachine<T>::addStateAction(uint8_t state,  StateActionFunction stateActionFunction){
  _stateActionMap[state] = stateActionFunction;
}

template<class T>
void FiniteStateMachine<T>::addStateEntryAction(uint8_t state,  StateActionFunction stateEntryActionFunction){
  _stateEntryActionMap[state] = stateEntryActionFunction;
}

template<class T>
void FiniteStateMachine<T>::addStateExitAction(uint8_t state,  StateActionFunction stateExitActionFunction){
  _stateExitActionMap[state] = stateExitActionFunction;
}

template<class T>
void FiniteStateMachine<T>::triggerTransition(uint8_t fromState, uint8_t toState){
  _transitionTriggerMap[fromState] = toState;
}

template<class T>
uint8_t FiniteStateMachine<T>::getState(){
  return _state;
}

template<class T>
void FiniteStateMachine<T>::loop(){

  auto it1 = _transitionTriggerMap.find(_state);
  // transitionConfFunctions found
  if (it1 != _transitionTriggerMap.end()){
    uint8_t toState = it1->second;
    _transit(toState);
    _transitionTriggerMap.erase(it1);
  }
  else {
    // check whether we have to move to new state
    // pick transition for current state
    auto it2 = _transitionMap.find(_state);
    // transitionConfFunctions found
    if (it2 != _transitionMap.end()){
      std::map<uint8_t,TransitionCondFunction> innerTransMap = it2->second;
      for(typename std::map<uint8_t,TransitionCondFunction>::iterator it3 = innerTransMap.begin();
          it3 != innerTransMap.end(); ++it3) {

        bool (T::*tf)(void)  = it3->second;
        // is transition function  fullfilled?
        if ((_obj.*tf)()) {
          uint8_t toState =  it3->first;
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
    FLASH_PRINTF2("FSM<%s>: state action %d\n" , _debugString.c_str(), _state);
#endif
    void (T::*saf)(void)  = iter->second;
    (_obj.*saf)();
  }
}

template<class T>
void FiniteStateMachine<T>::_transit(uint8_t toState){
  // call exit function of last state
  auto it1 = _stateExitActionMap.find(_state);
  if (it1 != _stateExitActionMap.end()){
#ifdef FSM_DEBUG
    FLASH_PRINTF2("FSM<%s>: state exit %d\n", _debugString.c_str(), _state);
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
        FLASH_PRINTF2("FSM<%s>: state entry %d\n", _debugString.c_str(), _state);
#endif
    void (T::*sentryaf)(void)  = it2->second;
    (_obj.*sentryaf)();
  }
}

#endif
