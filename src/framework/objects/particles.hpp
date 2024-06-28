#ifndef PARTICLES_H
#define PARTICLES_H

#include <iostream>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include <sprites.hpp>
#include <tilemap_entity.hpp>
#include <misc.hpp>
#include <entity.hpp>
#include <transform_component.hpp>
#include <misc.hpp>

#include <json.hpp>
#include <memory>
#include <fstream>
using json = nlohmann::json;

#define PARTICLE_DIR (std::string)"assets/particles/"+

// <Particle Manager>
typedef std::shared_ptr<json> ParticleDataPtr;

/// @brief Takes care of loading and dynamically unloading partcle data
class ParticleDataManager {
public:
    static std::map<
        std::string,
        ParticleDataPtr
    > particle_data_map;

    static float timer;
    static float tick;

    /// @brief Load particle data into memory
    /// @param path Path to data
    static void load(std::string path);
    /// @brief Unloads particle data from memory
    /// @param path Path to data
    static void unload(std::string path);
    /// @brief Returns smart pointer to particle data
    /// @param path Path to data
    /// @return Smart pointer to particle data
    static ParticleDataPtr get(std::string path);

    static void unload_check();
    static void unload_unused();
    static void unload_all();
    static void reload();
};

/// @brief Shape that determines the offset position of particles
class EmitShape {
public:
    /// @brief When this is set to 1 the particles spawn on the edge of a shape, when it's set to 0 they spawn anywhere inside the shape
    float edge_ratio;
    EmitShape(float edge_ratio);

    virtual Vector2 get_pos(Vector2 position, float scale) = 0;
};

/// @brief Particles will emit in a circle
/// @note This shape is used when the "shape" property in the particle data has 2 paramaters (radius and edge ratio)
class EmitCircle: public EmitShape {
public:
    float radius;
    EmitCircle(float radius, float edge_ratio);

    Vector2 get_pos(Vector2 position, float scale);
};

/// @brief Particles will emit in a rectangle
/// @note This shape is used when the "shape" property in the particle data has 3 paramaters (radius and edge ratio)
class EmitRect: public EmitShape {
public:
    Vector2 dimensions;
    EmitRect(Vector2 dimensions, float edge_ratio);

    /// @brief Returns a random position for a particle
    /// @param position Position of particle
    /// @param scale Scale of particle
    /// @return Returns a new position with a random offset
    Vector2 get_pos(Vector2 position, float scale);
};

/// @brief Particles will emit in a single point
/// @note This shape is used when the "shape" property in the particle data doesn't exist
class EmitPoint: public EmitShape {
public:
    EmitPoint();
    Vector2 get_pos(Vector2 position, float scale);
};

/// @brief Draws and processes list of particles
class ParticleSystem: public Drawable {
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

    Vector2 force;
    float spawn_timer;
    int left;

    // All particle system properties (loaded from a JSON file)
    EmitShape* emit_shape;

    float lifetime, lifetime_randomness;
    float particle_angle,
          particle_angle_randomness;

    bool rotate_to_velocity;
    float angular_velocity, angular_velocity_randomness;
    float velocity, velocity_randomness, velocity_end, shot_angle, spread;

    float particle_scale,
          particle_scale_randomness,
          particle_scale_end;

    float firerate, firerate_randomness;
    int amount;
    Color particle_tint, particle_tint_end; float particle_tint_randomness;
    
    std::string velocity_ease_name;
    std::string scale_ease_name;
    std::string tint_ease_name;

    std::shared_ptr<Texture2D> texture;

public:
    ParticleSystem(std::string data_filename, Vector2 position={0, 0});
    ~ParticleSystem();

    /// @brief Returns force vector that acts upon particles
    /// @return Force vector
    Vector2 get_force();
    /// @brief Adds to the force vector that affects particles
    /// @param adding Vector to add
    void add_force(Vector2 adding);
    /// @brief Subtracts from the force vector
    /// @param removing Force subtracting
    void remove_force(Vector2 removing);

    /// @brief Sets the tilemap particles collide with
    /// @param mask Pointer to tilemap entity
    void set_collision_mask(Tilemap *mask);
    /// @brief Returns the tilemap the particles collide with
    /// @return Pointer to tilemap
    Tilemap *get_collision_mask();

    /// @brief Sets the position of the tilemap
    /// @param position Vector the position is set to
    void set_position(Vector2 position);
    /// @brief Returns the position of the tilemap
    /// @return Tilemaps position
    Vector2 get_position();

    /// @brief Sets the number of particle bursts left to emit
    /// @param new_left Number of particle bursts left to emit (-1 = infinite)
    void set_left(int new_left);
    /// @brief Returns the amount of bursts left to emit
    /// @return Amount of bursts left to emit (-1 = infinite)
    int get_left();
    /// @brief Returns number of particles that are alive
    /// @return Number of particles that are alive
    int get_num_particles();

    /// @brief Sets the amount of particles emitted in a burst
    /// @param amt Amount of particles emitted in a burst
    void set_amount(int amt);

    void reload_data();

    void process(float delta);
    void draw();
};
/// @brief Entity that holds a particle system and unloads itself automatically once there arent any particles left
class ParticleEntity: public Entity {
public:
    ParticleSystem system;

    ParticleEntity(std::string path, Vector2 pos, int left=1, Tilemap *mask=nullptr);
    void process(float delta);
};

#endif