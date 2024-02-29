#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include <raylib.h>
#include <raymath.h>
#include <FastNoiseLite.h>

#include <component.hpp>
#include <entity.hpp>

#include <transform_component.hpp>
#include <misc.hpp>

extern Camera2D *global_camera;
extern FastNoiseLite shake_noise_obj;

namespace CameraManager {
    void bind_camera(Camera2D *new_camera);
    Camera2D *get_camera();
    void init();
};

// <Camera Component>
class CameraComponent: public Component {
private:
    Vector2 desired_position, position, offset, shake_offset;
    float smoothing_speed;

    float shake_strength, shake_duration, shake_timer;
    Vector2 shake_direction;
    
    Camera2D camera;
public:
    CameraComponent(Entity *entity, Vector2 position={0,0});

    Camera2D *get_camera();

    void shake(float strength, float duration, float direction=-1);
    
    void process(float delta);
};

#endif