#include <camera_component.hpp>

// <Camera Manager>
Camera2D *global_camera = nullptr;
FastNoiseLite shake_noise_obj = FastNoiseLite();

void CameraManager::bind_camera(Camera2D *new_camera) {
    global_camera = new_camera;
}
Camera2D *CameraManager::get_camera() {
    return global_camera;
}

void CameraManager::init() {
    shake_noise_obj.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
}

// <Camera Component>
CameraComponent::CameraComponent(Entity *entity, Vector2 position):
    Component(CompType::CAMERA, entity),
    desired_position {position},
    position {position},
    offset {0, 0},
    shake_offset {0, 0},
    smoothing_speed {10},
    shake_strength {0},
    shake_duration {1},
    shake_timer {0}
{
    camera.offset = position;
}

void CameraComponent::process(float delta) {
    if (&camera != global_camera) return;

    // Update desired position based on entity's transform component
    if (entity->has_component(CompType::TRANSFORM)) {
        TransformComponent *comp = (TransformComponent*)entity->get_component(CompType::TRANSFORM);
        desired_position = comp->position;
    }

    // Smoothly interpolate position
    position = Lerpi(position, desired_position, smoothing_speed);

    // Calculating shake value
    shake_timer = std::max(shake_timer - delta, 0.f);

    float shake_noise = abs(shake_noise_obj.GetNoise((float)GetTime() * 1000.f, 1.f));
    shake_noise *= shake_strength;
    shake_noise *= pow(sin(PI * (shake_timer / shake_duration)), 2.0);

    shake_offset = Vector2Multiply(shake_direction, {shake_noise, shake_noise});

    // Updating camera data
    camera.target = Vector2Add(position, offset); // Position + Offset
    camera.target = Vector2Subtract(camera.target, shake_offset); // Shake

    camera.target = Vector2Subtract(camera.target, half_res); // Center

    camera.rotation = 0;
    camera.zoom = 1;
    camera.offset = {0, 0};
}

Camera2D *CameraComponent::get_camera() {
    return &camera;
}

void CameraComponent::shake(float strength, float duration, float angle) {
    if (shake_timer > 0 && shake_strength >= strength) {
        return;
    }

    if (angle == -1) {
        angle = RandF()*PI*2;
    }

    shake_strength = strength;
    shake_direction = Vector2Rotate({1, 0}, angle);
    
    shake_timer = duration;
    shake_duration = duration;
}