#include <camera_component.hpp>

// <Camera Manager>
Camera2D *camera = nullptr;

void CameraManager::bind_camera(Camera2D *new_camera) {
    camera = new_camera;
}
Camera2D *CameraManager::get_camera() {
    return camera;
}

// <Camera Component>
CameraComponent::CameraComponent(Entity *entity, Vector2 position):
    Component(CompType::CAMERA, entity),
    desired_position {position},
    position {position},
    offset {0, 0},
    smoothing_speed {5}
{
    camera.offset = position;
}

void CameraComponent::process(float delta) {
    // Update desired position based on entity's transform component
    if (entity->has_component(CompType::TRANSFORM)) {
        TransformComponent *comp = (TransformComponent*)entity->get_component(CompType::TRANSFORM);
        desired_position = comp->position;
    }

    position = Lerpi(position, desired_position, smoothing_speed);

    camera.target = Vector2Add(position, offset);
    camera.target = Vector2Subtract(camera.target, half_res);

    camera.rotation = 0;
    camera.zoom = 1;
    camera.offset = {0, 0};
}

Camera2D *CameraComponent::get_camera() {
    return &camera;
}