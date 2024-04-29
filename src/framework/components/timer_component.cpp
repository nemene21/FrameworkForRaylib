#include <timer_component.hpp>

Timer::Timer(std::string name, float duration, Entity *entity):
    duration {duration},
    progress {0},
    repeat {false},
    paused {false},
    running {false},
    finished {Signal()},
    name {name},
    entity {entity} {
        if (duration < ( TIMER_LIMIT )) {
            std::cout << "WARNING - Timer is too fast" << std::endl;
        }
    }

void Timer::start() {
    if (running == true) return;
     
    progress = 0;
    running = true;
}

void Timer::restart() {
    progress = 0;
    running = true;
}

void Timer::pause()        {paused = true; }
void Timer::unpause()      {paused = false;  }
void Timer::toggle_pause() {paused = ! paused; }

void Timer::process(float delta) {
    progress += delta * (float)(!paused) * (float)(running);

    if (progress >= duration) {
        progress -= duration;
        running   = repeat;

        if (!running) progress = 0;

        finished.emit(entity);
    }
}

TimerComponent::TimerComponent(Entity *entity):
    Component(CompType::TIMER, entity),
    timers {} {}

TimerComponent::~TimerComponent() {
    for (auto &timer_pair: timers) {
        free(timer_pair.second);
    }
}

void TimerComponent::process(float delta) {
    for (auto &timer_pair: timers) {
        timer_pair.second->process(delta);
    }
}

Timer *TimerComponent::add_timer(std::string name, float duration, bool start) {
    Timer *timer = new Timer(name, duration, entity);

    if (start) timer->start();

    timers[name] = timer;
    return timer;
}

Timer *TimerComponent::get_timer(std::string name) {
    if (timers.find(name) == timers.end()) {
        std::cout << "ERROR - Timer " << name << " not found!" << std::endl;
        exit(1);
    }

    return timers[name];
}