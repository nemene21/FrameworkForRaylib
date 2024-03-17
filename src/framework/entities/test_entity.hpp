#ifndef TEST_ENTITY_H
#define TEST_ENTITY_H

#include <sprites.hpp>
#include <trails.hpp>
#include <particles.hpp>
#include <tilemap_entity.hpp>
#include <audio.hpp>
#include <scene.hpp>
#include <health_component.hpp>
#include <transform_component.hpp>
#include <camera_component.hpp>
#include <animation_component.hpp>

class TestEntity: public Entity {
protected:
    Sprite sprite;
    ParticleSystem particle_sys;
    Trail trail_vfx;

public:
    TestEntity();

    void stretch(float anim);
    void squash(float anim);
    void return_state(float anim);
    void spin(float anim);
    
    void process(float delta);
    void draw(float delta);
};

#endif