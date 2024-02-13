#include <particles.h>

// <Particle Data Manager>
std::map<std::string, ParticleDataPtr> ParticleDataManager::particle_data_map;
float ParticleDataManager::timer = 1.0f;
float ParticleDataManager::tick  = 100.0f;

// Load particle data and put it's smart pointer into the particle data map (it's path is the key)
void ParticleDataManager::load(std::string name) {
    std::ifstream f(PARTICLE_DIR name);

    if (!f.is_open())
        std::cerr << "Can't open file " + PARTICLE_DIR name + " :(" << std::endl;

    json data = json::parse(f);
    particle_data_map[name] = std::make_shared<json>(
        data
    );
}

// Returns particle data smart pointer and loads the particle data if required
ParticleDataPtr ParticleDataManager::get(std::string name) {
    if (particle_data_map.find(name) != particle_data_map.end())
        return particle_data_map[name];

    load(name);
    return particle_data_map[name];
}

// Unloads particle data
void ParticleDataManager::unload(std::string name) {
    particle_data_map.erase(name);
}

// Unloads all particle data that isn't being referanced 
void ParticleDataManager::unload_unused() {
    for (auto& texture_pair: particle_data_map) {

        if (texture_pair.second.use_count() == 1) {
            unload(texture_pair.first);
        }
    }
}
// Ticks down a timer which calls "unload_unused()" when it hits 0 every "tick" seconds
void ParticleDataManager::unload_check() {
    timer -= GetFrameTime();

    if (timer < .0) {
        timer = tick;

        unload_unused();
    }
}

void ParticleDataManager::reload() {
    for (auto& particle_pair: particle_data_map) {

        std::ifstream f(PARTICLE_DIR particle_pair.first);
        *particle_pair.second.get() = json::parse(f);
    }
}

// <Particle System>

void ParticleSystem::set_position(Vector2 new_position) {
    position = new_position;
}

Vector2 ParticleSystem::get_position() {
    return position;
}

void ParticleSystem::add_force(Vector2 adding) {
    force = Vector2Add(force, adding);
}

void ParticleSystem::remove_force(Vector2 removing) {
    force = Vector2Subtract(force, removing);
}

void ParticleSystem::reload_data() {
    json data = *particle_data.get();

    texture = TextureManager::get(data["texture"]);
    angle = data["angle"];
    angle_randomness = data["angle_randomness"];

    angular_velocity = data["angular_velocity"];
    angular_velocity_randomness = data["angular_velocity_randomness"];

    scale = data["scale"];
    scale_randomness = data["scale_randomness"];
    
    velocity = data["velocity"];
    shot_angle = data["shot_angle"];
    spread = data["spread"];
    firerate = data["firerate"];
    
    tint_randomness = data["tint_randomness"];

    int r = data["tint"][0], g = data["tint"][1], b = data["tint"][2], a = data["tint"][3];

    tint = Color{
        static_cast<unsigned char>(r),
        static_cast<unsigned char>(g),
        static_cast<unsigned char>(b),
        static_cast<unsigned char>(a)
    };

    // tint = WHITE;
}

ParticleSystem::ParticleSystem(std::string data_filename, Vector2 position): position {position} {
    
    particle_data = ParticleDataManager::get(data_filename);
    reload_data();
}

void ParticleSystem::spawn_particle() {
    json data = *particle_data.get();

    Particle new_particle;
    new_particle.position = position;
    new_particle.scale = scale + (scale_randomness*.5 * RandF2());

    new_particle.angle = angle + (angle_randomness*.5 * RandF2());
    new_particle.angular_velocity = angular_velocity  + (angular_velocity_randomness*.5 * RandF2());

    new_particle.tint  = Color{
        static_cast<unsigned char>(tint.r + tint_randomness*.5 * RandF2()),
        static_cast<unsigned char>(tint.g + tint_randomness*.5 * RandF2()),
        static_cast<unsigned char>(tint.b + tint_randomness*.5 * RandF2()),
        tint.a
    };

    new_particle.lifetime_max = lifetime;
    new_particle.lifetime     = lifetime;

    new_particle.velocity = Vector2Rotate({velocity, 0}, shot_angle + RandF2() * spread*.5);

    particles.push_back(new_particle);
}

void ParticleSystem::process(float delta) {
    spawn_timer -= delta;

    if (spawn_timer <= 0) {
        spawn_timer = 1.0 / firerate;
        spawn_particle();
    }

    for (auto& particle: particles) {
        particle.velocity = Vector2Add(particle.velocity,
            Vector2Multiply(force, {delta, delta})
        );

        particle.position = Vector2Add(particle.position,
            Vector2Multiply(particle.velocity, {delta, delta})
        );

        particle.angle += particle.angular_velocity * delta;
    }
}

void ParticleSystem::draw() {
    for (auto& particle: particles) {

        DrawTextureCentered(texture.get(),
            particle.position,
            {particle.scale, particle.scale},
            particle.angle,
            particle.tint
        );
    }
}