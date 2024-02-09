#include <particles.h>

// <Particle Data Manager>
std::map<std::string, ParticleDataPtr> ParticleDataManager::particle_data_map;
float ParticleDataManager::timer = 0.0f;
float ParticleDataManager::tick  = 100.0f;

// Load particle data and put it's smart pointer into the particle data map (it's path is the key)
void ParticleDataManager::load(std::string name) {
    std::ifstream f(PARTICLE_DIR name);

    particle_data_map[name] = std::make_shared<json>(
        json::parse(f)
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

ParticleSystem::ParticleSystem(std::string data_filename) {
    particle_data = ParticleDataManager::get(data_filename);

    reload_data();
}

void ParticleSystem::reload_data() {
    
}

void ParticleSystem::draw() {
    for (auto& particle: particles) {

        // Draw texture
        Texture2D* texture_ptr = texture.get();
        float width  = texture_ptr->width  * particle.scale,
            height = texture_ptr->height * particle.scale;

        DrawTexturePro(
            *texture_ptr,

            Rectangle{
                0, 0,
                (float)texture_ptr->width,
                (float)texture_ptr->height
            },
            Rectangle{

                particle.position.x, particle.position.y,
                width,
                height
            },
            
            Vector2{width * .5f, height * .5f},
            particle.angle,
            particle.tint
        );
    }
}