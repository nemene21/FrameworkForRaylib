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
#include <area_component.hpp>
#include <timer_component.hpp>
#include <input.hpp>

void track1(Entity *entity);
void track2(Entity *entity);

class TestEntity: public Entity {
protected:
    Sprite sprite;
    ParticleSystem particle_sys;
    Trail trail_vfx;
    bool track_bool;

    Timer *test_timer;

public:
    TestEntity();
    
    void process(float delta);
    void draw(float delta);
};

#endif