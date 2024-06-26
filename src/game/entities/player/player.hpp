#ifndef PLAYER_H
#define PLAYER_H

#include <entity.hpp>
#include <transform_component.hpp>
#include <area_component.hpp>
#include <sprites.hpp>
#include <particles.hpp>
#include <input.hpp>
#include <entities/bubble/bubble.hpp>
#include <scenes/game.hpp>

class Player: public Entity {
public:
    TransformComponent* trans_comp;
    Sprite sprite;
    ParticleSystem move_particles;

    float shot_charge;

    Player();
    void process(float delta);
    void shoot(float angle);
};

#endif