#ifndef WITHIN_UNITTEST
  #include <ArduinoSTL.h>
#endif

#include <map>

using namespace std;

typedef bool (*TransFunc)(void);
typedef void (*StateActionFunc)(void);

class FinisteStateMachine {
public:
  FinisteStateMachine(int numberOfStates, int initialState);
  void addTransition(int fromState, int toState,  TransFunc transitionFunction );
  void addStateAction(int state, void (*stateAction)(void));

  void loop();

  void setState(int state);
  int getState();

private:
  int _numStates;
  int _state;
  map<int,map<int,TransFunc>> _transitionMap;
  map<int,StateActionFunc> _stateActionMap;

};
