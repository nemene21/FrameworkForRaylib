#ifndef BUBBLE_H
#define BUBBLE_H

#include <entity.hpp>
#include <transform_component.hpp>
#include <area_component.hpp>
#include <sprites.hpp>
#include <particles.hpp>
#include <scene.hpp>

class Bubble: public Entity {
public:
    TransformComponent* trans_comp;
    Sprite sprite;
    int bounces_left;

    void pop();

    Bubble(Vector2 position, Vector2 velocity, float charge);
    void process(float delta);
};

#endif