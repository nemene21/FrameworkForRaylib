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

/// @brief Manages current camera and provides a convenient referance to it
namespace CameraManager {
    void bind_camera(Camera2D *new_camera);
    Camera2D *get_camera();
    void init();
};

/// @brief Component to handle a camera, follows an entity and has inbuilt zoom and shake animations
class CameraComponent: public Component {
private:
    Vector2 desired_position, shake_offset;
    float smoothing_speed, shake_rotation;

    float camera_zoom, zoom_anim_timer, zoom_anim_duration, zoom_animation_value;

    float shake_strength, shake_duration, shake_timer;
    Vector2 shake_direction;
    
    Camera2D camera;
public:
    Vector2 position, offset;

    CameraComponent(Entity *entity, Vector2 position={0,0});

    /// @brief Returns pointer to component's camera
    /// @return Pointer to camera
    Camera2D *get_camera();
    
    /// @brief Zoom in/out effect
    /// @param new_zoom Zoom that you are animating to
    /// @param duration Duration of zoom animation
    void zoom(float new_zoom, float duration);
    /// @brief Shake effect
    /// @param strength Distance in pixels the camera will shake
    /// @param duration 
    /// @param direction Shake angle
    void shake(float strength, float duration, float direction=-1);
    
    void set_zoom(float zoom);    
    void process(float delta);
};

#endif