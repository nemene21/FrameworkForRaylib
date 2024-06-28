#ifndef PLAYER_H
#define PLAYER_H
#include <entity.hpp>
#include <sprites.hpp>
#include <transform_component.hpp>
#include <input.hpp>

class Player: public Entity {
public:
    Sprite sprite;

    Player();
    void process(float delta);
};

#endif