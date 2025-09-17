#include "StateMachine.hpp"

// registers the current state
void StateMachine::registerState(StateID id, Factory factory) {
    factories[id] = std::move(factory);
}

// changes to next state based on transition
void StateMachine::changeState(StateID id) {
    auto it = factories.find(id);
    if (it == factories.end()) return;
    if (currentState) currentState->onExit();
    currentState = it->second(ctx);
    if (currentState) currentState->onEnter();
}

