#ifndef SIGNAL_H
#define SIGNAL_H
#include <iostream>
#include <functional>
#include <vector>

class Entity;
typedef std::function<void(Entity *)> SignalFunction;


/// @brief Calls callbacks when a certain event happens
class Signal {
protected:
    std::vector<SignalFunction> callers;

public:
    Signal();
    /// @brief Connect a callback function to the signal
    /// @param func Pointer to callback
    /// @note Callbacks return nothing and have only one parameter (Entity*)
    void connect(SignalFunction func);
    /// @brief Calls all the callbacks (event happened)
    /// @param parent The entity the callbacks referance
    void emit(Entity* parent);
};

#endif