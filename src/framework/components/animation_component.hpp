#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <component.hpp>
#include <functional>
#include <map>
#include <misc.hpp>

typedef std::function<void(float)> KeyFunc;

class AnimationComponent: public Component {
protected:
    bool paused;
    float direction, playback_speed;

    std::string animation_playing;

    typedef struct {
        std::function<void(float)> function;
        float start, end;

    } Keyframe;

    typedef struct {
        std::function<void(float)> function;
        float time;
        bool played;
    } Event;

    typedef struct {
        std::vector<Keyframe> keyframes;
        std::vector<Event> events;
        float duration, progress;

        bool repeating;

    } Animation;

    std::map<std::string, Animation> animation_map;

    void process_animation(std::string anim, float delta);

public:
    typedef std::map<std::string, Animation> AnimationMap;

    AnimationComponent(Entity *entity);

    Signal animation_finished, animation_started;

    void play(std::string name);

    void process(float delta);

    void make_animation(std::string name, float duration, bool repeating);
    void add_keyframe(std::string name, float start, float end, std::function<void(float)> function);
    void add_event(std::string name, float time, std::function<void(float)> function);

    void toggle_pause();
    void pause();
    void unpause();
};

#endif