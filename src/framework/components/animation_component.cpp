#include <animation_component.hpp>

AnimationComponent::AnimationComponent(Entity *entity): Component(CompType::ANIMATION, entity),
    paused {false},
    direction {1},
    playback_speed {1}
    {}

void AnimationComponent::make_animation(std::string name, float duration, bool repeating) {
    
    Animation new_animation;
    new_animation.repeating = repeating;
    new_animation.keyframes = {};
    
    new_animation.progress = 0;
    new_animation.duration = duration;

    animation_map[name] = new_animation;
}

void AnimationComponent::add_keyframe(std::string name, float start, float end, std::function<void(float)> function) {

    Keyframe new_keyframe;
    new_keyframe.start = start;
    new_keyframe.end = end;

    new_keyframe.function = function;

    animation_map[name].keyframes.push_back(new_keyframe);
}

void AnimationComponent::add_event(std::string name, float time, std::function<void(float)> function) {

    Event new_event;
    new_event.time = time;

    new_event.function = function;
    new_event.played = false;

    animation_map[name].events.push_back(new_event);
}


void AnimationComponent::process_animation(std::string name, float delta) {
    Animation &animation = animation_map[name];

    animation.progress += delta * direction * playback_speed * int(!paused);

    for (auto &keyframe: animation.keyframes) {
        if (animation.progress > keyframe.start && animation.progress < keyframe.end)
        
            keyframe.function((animation.progress - keyframe.start) / (keyframe.end - keyframe.start));
    }
    
    for (auto &event: animation_map[name].events) {
        if (!event.played && event.time >= animation.progress) {

            event.function(animation.progress / animation.duration);
            event.played = true;
        }
    }

    if (animation.progress > animation.duration && animation.repeating) {
        animation.progress = 0;
        
        for (auto &event: animation_map[name].events) {
            event.played = false;
        }
    }
}

void AnimationComponent::play(std::string name) {

    animation_playing = name;

    for (auto &event: animation_map[name].events) {
        event.played = false;
    }
}

void AnimationComponent::process(float delta) {
    process_animation(animation_playing, delta);
}