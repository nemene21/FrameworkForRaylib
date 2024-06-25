#ifndef TIMER_H
#define TIMER_H
#include <map>
#include <component.hpp>

#define TIMER_LIMIT (float)(1./60.f)

class Timer {
public:
    float duration, progress;
    bool repeat, paused, running;
    Signal finished;
    std::string name;
    Entity *entity;

    Timer(std::string name, float duration, Entity *entity);

    void start();
    void restart();

    void pause();
    void unpause();
    void toggle_pause();

    void process(float delta);
};

class TimerComponent: public Component {
public:
    std::map<std::string, Timer *> timers;

    TimerComponent(Entity *entity);
    ~TimerComponent();

    Timer *add_timer(std::string name, float duration, bool start=false);
    Timer *get_timer(std::string name);

    void process(float delta);
};

#endif