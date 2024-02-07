#ifndef PARTICLES_H
#define PARTICLES_H

#include <iostream>
#include <vector>
#include <raylib.h>
#include <raymath.h>

typedef struct {
    Vector2 position;
    float strength, range;
} ParticleAttractor;

Vector2 CalculateAttractor(ParticleAttractor &attractor, Vector2 pos);

class ParticleSystem {
protected:
    typedef struct {
        Vector2 position, velocity;
        float lifetime, lifetime_max, angle, angular_velocity;

    } Particle;

    void process_particle(Particle& particle);

    typedef std::vector<Particle> ParticleList;
    ParticleList particles;

    Vector2 force;
    std::vector<ParticleAttractor*> &attractors;

public:
    ParticleSystem(std::string data);

    void add_force(Vector2 adding);
    void remove_force(Vector2 removing);

    void process();
    void draw();
};

#endif