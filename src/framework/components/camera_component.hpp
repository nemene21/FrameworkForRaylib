#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include <raylib.h>
#include <raymath.h>

#include <component.hpp>
#include <entity.hpp>

#include <transform_component.hpp>
#include <misc.hpp>

extern Camera2D *camera;

namespace CameraManager {
    void bind_camera(Camera2D *new_camera);
    Camera2D *get_camera();
};

// <Camera Component>
class CameraComponent: public Component {
private:
    Vector2 desired_position, position, offset;
    float smoothing_speed;
    Camera2D camera;
public:
    CameraComponent(Entity *entity, Vector2 position={0,0});

    Camera2D *get_camera();
    
    void process(float delta);
};

#endif