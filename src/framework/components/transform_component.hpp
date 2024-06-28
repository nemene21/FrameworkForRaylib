#ifndef TRANSFORM_COMP_H
#define TRANSFORM_COMP_H

#include <component.hpp>
#include <entity.hpp>
#include <raylib.h>
#include <raymath.h>
#include <misc.hpp>
#include <collider_component.hpp>
#include <area_component.hpp>

/// @brief Component responsible for postion, movement, scale and angle of an entity
class TransformComponent: public Component {
public:
    Vector2 position, scale, velocity;
    float angle;

    TransformComponent(Entity *entity, Vector2 position = {0, 0}, Vector2 scale = {1, 1}, float angle = 0);

    /// @brief Adds vector to position
    /// @param adding Vector to add
    void translate(Vector2 adding);
    /// @brief Adds to x position
    /// @param adding X to add
    void translate_x(float adding);
    /// @brief Adds to y position
    /// @param adding Y to add
    void translate_y(float adding);

    /// @brief Smoothly interpolates the velocity of the transform
    /// @param to The vector to interpolate the velocity to
    /// @param speed The speed of the interpolation
    /// @note This is frame independant
    void interpolate_velocity(Vector2 to, float speed);
    /// @brief Add a speed to the velocity
    /// @param by Speed to add
    void accelerate(Vector2 by);

    void process(float delta);
    void check_bounds(Vector2 direction);
};

#endif