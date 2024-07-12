#ifndef TIMER_H
#define TIMER_H
#include <map>
#include <component.hpp>
#include <math.h>

#define TIMER_LIMIT (float)(1./60.f)

/// @brief Simple timer
class Timer {
public:
    float duration, progress;
    /// @brief If true, the timer automatically restarts
    bool repeat;
    /// @brief If true, the timer is stopped for the time being
    bool paused;
    /// @brief If true, the timer is currently ticking down
    bool running;

    /// @brief Signal is emitted upon the timer finishing
    Signal finished;
    std::string name;
    Entity *entity;

    Timer(std::string name, float duration, Entity *entity);

    /// @brief Starts timer
    void start();
    /// @brief Restarts timer
    void restart();

    /// @brief Pauses the timer
    void pause();
    /// @brief Unpauses the timer
    void unpause();
    /// @brief Flips the pause value
    void toggle_pause();

    /// @brief Ticks down timer
    /// @param delta Time to tick
    void process(float delta);
};

/// @brief Component that automatically updates timers and provides easy access to them
class TimerComponent: public Component {
public:
    std::map<std::string, Timer *> timers;

    TimerComponent(Entity *entity);
    ~TimerComponent();

    void draw_gui_info();

    /// @brief Adds a new timer to be automatically processed
    /// @param name Timer name
    /// @param duration Timer duration (in seconds)
    /// @param start If true the timer starts instantly upon being added
    /// @return Returns a pointer to the created timer
    /// @note The timer component is responsible for the memory of the added timer, there is no need to free it yourself
    Timer *add_timer(std::string name, float duration, bool start=false);
    /// @brief Returns a pointer to requested timer
    /// @param name Timer's name, specified upon being added
    /// @return Pointer to requested timer
    Timer *get_timer(std::string name);

    void process(float delta);
};

#endif