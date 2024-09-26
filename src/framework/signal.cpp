#include <signal.hpp>

// <Signals>
Signal::Signal(): callers {} {}

void Signal::connect(SignalFunction function) {
    callers.push_back(function);
}

void Signal::emit(Entity *parent) {

    for (SignalFunction caller: callers) {
        caller(parent);
    }
}