#ifndef TRANSFORM_COMP_H
#define TRANSFORM_COMP_H

#include <component.hpp>
#include <raylib.h>
#include <raymath.h>

class TransformComponent: public Component {
public:
    Vector2 position, scale, velocity;
    float angle;

    TransformComponent(Entity *entity, Vector2 position = {0, 0}, Vector2 scale = {1, 1}, float angle = 0);

    void translate(Vector2 adding);
    void translate_x(float adding);
    void translate_y(float adding);

    void process(float delta);
};

#endif