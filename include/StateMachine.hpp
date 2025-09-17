#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include "State.hpp"
#include <functional>
#include <unordered_map>

struct Context;

class StateMachine {
public:
    explicit StateMachine(Context& ctx) : ctx(ctx) {}

    // alias for factory design pattern
    using Factory = std::function<std::unique_ptr<State>(Context&)>;

    void registerState(StateID id, Factory factory);
    void changeState(StateID id);

    State* current() { return currentState.get(); }

private:
    Context& ctx;
    std::unordered_map<StateID, Factory> factories;
    // smart pointer for memory management
    std::unique_ptr<State> currentState;
};

#endif
