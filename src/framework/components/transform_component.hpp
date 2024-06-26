#ifndef TRANSFORM_COMP_H
#define TRANSFORM_COMP_H

#include <component.hpp>
#include <entity.hpp>
#include <raylib.h>
#include <raymath.h>
#include <misc.hpp>
#include <collider_component.hpp>
#include <area_component.hpp>

class TransformComponent: public Component {
public:
    Vector2 position, scale, velocity;
    float angle;

    TransformComponent(Entity *entity, Vector2 position = {0, 0}, Vector2 scale = {1, 1}, float angle = 0);

    void translate(Vector2 adding);
    void translate_x(float adding);
    void translate_y(float adding);

    void interpolate_velocity(Vector2 to, float speed);
    void accelerate(Vector2 by);

    void process(float delta);
    void check_bounds(Vector2 direction);
};

#endif