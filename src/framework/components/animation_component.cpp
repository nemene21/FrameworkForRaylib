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

void AnimationComponent::process_animation(std::string name, float delta) {
    Animation &animation = animation_map[name];

    animation.progress += delta * direction * playback_speed * int(!paused);

    for (auto &keyframe: animation.keyframes) {
        if (animation.progress > keyframe.start && animation.progress < keyframe.end)
        
            keyframe.function((animation.progress - keyframe.start) / (keyframe.end - keyframe.start));
    }

    if (animation.progress > animation.duration && animation.repeating) {
        animation.progress = 0;
    }
}

void AnimationComponent::play(std::string name) {

    animation_playing = name;
}

void AnimationComponent::process(float delta) {
    process_animation(animation_playing, delta);
}