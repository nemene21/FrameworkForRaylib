#ifndef TEST_ENTITY_H
#define TEST_ENTITY_H

#include <framework.h>

class TestEntity: public Entity {
protected:
    Sprite sprite;
    ParticleSystem particle_sys;
    Trail trail_vfx;

public:
    TestEntity();
    
    void process(float delta);
    void draw(float delta);
};

#endif