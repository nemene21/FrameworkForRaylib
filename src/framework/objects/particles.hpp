#ifndef PARTICLES_H
#define PARTICLES_H

#include <iostream>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include <sprites.hpp>
#include <tilemap_entity.hpp>
#include <misc.hpp>

#include <json.hpp>
#include <memory>
#include <fstream>
using json = nlohmann::json;

#define PARTICLE_DIR (std::string)"assets/particles/"+

// <Particle Manager>
typedef std::shared_ptr<json> ParticleDataPtr;

class ParticleDataManager {
public:
    static std::map<
        std::string,
        ParticleDataPtr
    > particle_data_map;

    static float timer;
    static float tick;

    static void load(std::string path);
    static void unload(std::string path);
    static ParticleDataPtr get(std::string path);

    static void unload_check();
    static void unload_unused();
    static void unload_all();
    static void reload();
};

// <Particle Class>
class ParticleSystem {
protected:
    typedef struct {
        Vector2 position, velocity;
        float lifetime, lifetime_max, angle, angular_velocity, scale, scale_end;
        Color tint, tint_end;

    } Particle;

    Tilemap *collision_mask;

    ParticleDataPtr particle_data;

    void spawn_particle();
    void process_particle(Particle& particle);
    void attempt_particle_bounce(Particle& particle, Vector2 dir, float velocity_anim, float delta);

    typedef std::vector<Particle> ParticleList;
    ParticleList particles;

    Vector2 position, force;
    float spawn_timer;
    int left;

    // All particle system properties (loaded from a JSON file)
    float lifetime, lifetime_randomness;
    float angle, angle_randomness;
    bool rotate_to_velocity;
    float angular_velocity, angular_velocity_randomness;
    float velocity, velocity_randomness, velocity_end, shot_angle, spread;
    float scale, scale_randomness, scale_end;
    float firerate, firerate_randomness;
    int amount;
    Color tint, tint_end; float tint_randomness;
    
    std::string velocity_ease_name;
    std::string scale_ease_name;
    std::string tint_ease_name;

    std::shared_ptr<Texture2D> texture;

public:
    ParticleSystem(std::string data_filename, Vector2 position={0, 0});

    Vector2 get_force();
    void add_force(Vector2 adding);
    void remove_force(Vector2 removing);

    void set_collision_mask(Tilemap *mask);

    void set_position(Vector2 position);
    Vector2 get_position();

    void set_left(int new_left);
    int get_left();

    void reload_data();

    void process(float delta);
    void draw();
};

#endif