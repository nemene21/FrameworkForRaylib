#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <component.hpp>
#include <functional>
#include <map>
#include <misc.hpp>

typedef std::function<void(float)> KeyFunc;

/// @brief Component to handle animations with keyframes and events (animations, cutscenes...)
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

    /// @brief Singal that emits when an animation finishes
    Signal animation_finished;
    /// @brief Singal that emits when an animation ends
    Signal animation_started;

    /// @brief Play animation
    /// @param name Animation name
    void play(std::string name);

    void process(float delta);

    /// @brief Initialises an animation
    /// @param name Animation name
    /// @param duration 
    /// @param repeating 
    void make_animation(std::string name, float duration, bool repeating);
    /// @brief Adds keyframe to animation
    /// @param name Animation name
    /// @param start Keyframe start time
    /// @param end Keyframe end time
    /// @param function Animation function
    /// @note Keyframe start and end time are relative to the animation duration and are a value from 0, start of animation, to 1, end of animation
    void add_keyframe(std::string name, float start, float end, std::function<void(float)> function);
    /// @brief Add event to animation
    /// @param name Animation name
    /// @param time Event time
    /// @param function Event callback
    /// @note Event time is relative to the animation duration and is a value from 0, start of animation, to 1, end of animation
    void add_event(std::string name, float time, std::function<void(float)> function);

    /// @brief Toggles animation pause
    void toggle_pause();
    /// @brief Pauses animation
    void pause();
    /// @brief Unpauses animation
    void unpause();
};

#endif