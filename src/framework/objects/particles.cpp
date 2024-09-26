#include <particles.hpp>

// <Particle Data Manager>
std::map<std::string, ParticleDataPtr> ParticleDataManager::particle_data_map;
float ParticleDataManager::timer = 1.0f;
float ParticleDataManager::tick  = 100.0f;

// Load particle data and put it's smart pointer into the particle data map (it's path is the key)
void ParticleDataManager::load(std::string name) {
    FILE* file = fopen((PARTICLE_DIR name).c_str(), "rb");

    if (!file)
        std::cerr << "Can't open file " + PARTICLE_DIR name + " :(" << std::endl;

    char buffer[2048] = "";
    char line_buffer[128] = "";
    while (fgets(line_buffer, 2048, file)) {
        strcat(buffer, line_buffer);
    }

    fclose(file);
    json data = json::parse((std::string)buffer);
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
    std::vector<std::string> to_unload {};
    
    for (auto& texture_pair: particle_data_map) {
        if (texture_pair.second.use_count() == 1) {
            to_unload.push_back(texture_pair.first);
        }
    }
    for (auto to: to_unload) unload(to);
}
// Ticks down a timer which calls "unload_unused()" when it hits 0 every "tick" seconds
void ParticleDataManager::unload_check() {
    timer -= GetFrameTime();

    if (timer < .0) {
        timer = tick;

        unload_unused();
    }
}
// Hot reloading
void ParticleDataManager::reload() {
    for (auto& particle_pair: particle_data_map) {

        std::ifstream f(PARTICLE_DIR particle_pair.first);
        *particle_pair.second.get() = json::parse(f);
    }
}

// Unloads all particle data objects
void ParticleDataManager::unload_all() {
    std::vector<std::string> to_unload {};
    for (auto& particle_pair: particle_data_map) {
        to_unload.push_back(particle_pair.first);
    }
    for (auto to: to_unload) unload(to);
}

// <Emit Shapes>
EmitShape::EmitShape(float edge_ratio): edge_ratio {edge_ratio} {}

EmitCircle::EmitCircle(float radius, float edge_ratio):
    EmitShape(edge_ratio),
    radius {radius}
    {}

Vector2 EmitCircle::get_pos(Vector2 position, float scale) {
    float rng = Lerp(1, edge_ratio, (rand()%100) * 0.01f);
    Vector2 offset = Vector2Rotate({rng * radius, 0}, (rand()%628) * 0.01f);

    offset = Vector2Multiply(offset, {scale, scale});
    return Vector2Add(offset, position);
}

EmitRect::EmitRect(Vector2 dimensions, float edge_ratio):
    EmitShape(edge_ratio),
    dimensions {dimensions}
    {}

Vector2 EmitRect::get_pos(Vector2 position, float scale) {
    Vector2 offset;
    
    if (RandF() > .5f) {
        offset = {
            round(RandF()) - .5f,
            RandF() - .5f
        };
    } else {
        offset = {
            RandF() - .5f,
            round(RandF()) - .5f
        };
    }
    offset = Vector2Multiply(offset, dimensions);

    float coeff = fmaxf(0, RandF() - edge_ratio);
    offset = Lerp(offset, Vector2{0, 0}, coeff);

    offset = Vector2Multiply(offset, {scale, scale});
    return Vector2Add(offset, position);
}

EmitPoint::EmitPoint():
    EmitShape(0)
    {}

Vector2 EmitPoint::get_pos(Vector2 position, float scale) { return position; }

// <Particle System>

// Position setter and getter
void ParticleSystem::set_position(Vector2 new_position) {
    position = new_position;
}

Vector2 ParticleSystem::get_position() {
    return position;
}

// Forces
void ParticleSystem::add_force(Vector2 adding) {
    force = Vector2Add(force, adding);
}

void ParticleSystem::remove_force(Vector2 removing) {
    force = Vector2Subtract(force, removing);
}

Vector2 ParticleSystem::get_force() {
    return force;
}

// Collision mask setter
void ParticleSystem::set_collision_mask(Tilemap *mask) {
    collision_mask = mask;
}

// Collision mask getter
Tilemap *ParticleSystem::get_collision_mask() {
    return collision_mask;
}

// Left setter and getter
void ParticleSystem::set_left(int new_left) {
    left = new_left;
}
int ParticleSystem::get_left() {
    return left;
}

// Reload particle system object from it's json object
void ParticleSystem::reload_data() {
    json data = *particle_data.get();

    texture = TextureManager::get(data["texture"]);

    delete emit_shape;
    if (data.contains("shape")) {
        json shape_data = data["shape"];
        int num_params = shape_data.size();

        // Is circle (radius, edge)
        if (num_params == 2) {
            emit_shape = new EmitCircle(shape_data[0], shape_data[1]);
        
        // Is rectangle (width, height, edge)
        } else {
            emit_shape = new EmitRect({shape_data[0], shape_data[1]}, shape_data[2]);
        }
    // Default to point
    } else {
        emit_shape = new EmitPoint();
    }
    
    // Angle
    particle_angle = data["angle"];
    particle_angle_randomness = data["angle_randomness"];

    angular_velocity = data["angular_velocity"];
    angular_velocity_randomness = data["angular_velocity_randomness"];

    rotate_to_velocity = data["velocity_angle"];

    // Lifetime
    lifetime = data["lifetime"];
    lifetime_randomness = data["lifetime_randomness"];

    // Scale
    particle_scale = data["scale"];
    particle_scale_randomness = data["scale_randomness"];
    particle_scale_end = data["scale_end"];
    scale_ease_name = (std::string)(data["scale_ease"]);

    // Velocity
    velocity = data["velocity"];
    velocity_randomness = data["velocity_randomness"];
    velocity_end = data["velocity_end"];
    velocity_ease_name = (std::string)(data["velocity_ease"]);
    
    shot_angle = data["shot_angle"];
    spread = data["spread"];

    // Shooting
    firerate = data["firerate"];
    firerate_randomness = data["firerate_randomness"];
    
    amount = data["amount"];

    // Color
    tint_ease_name = (std::string)(data["tint_ease"]);

    particle_tint_randomness = data["tint_randomness"];

    int r = data["tint"][0], g = data["tint"][1], b = data["tint"][2], a = data["tint"][3];
    tint = Color{
        static_cast<unsigned char>(r),
        static_cast<unsigned char>(g),
        static_cast<unsigned char>(b),
        static_cast<unsigned char>(a)
    };

    particle_tint_end = Color{
        static_cast<unsigned char>(data["tint_end"][0]),
        static_cast<unsigned char>(data["tint_end"][1]),
        static_cast<unsigned char>(data["tint_end"][2]),
        static_cast<unsigned char>(data["tint_end"][3])
    };
}

// Constructor
ParticleSystem::ParticleSystem(std::string data_filename, Vector2 position):
    Drawable(position),
    collision_mask {nullptr},
    emit_shape {nullptr},
    force {0, 0},
    spawn_timer {0} {

    // Load data
    particle_data = ParticleDataManager::get(data_filename);
    reload_data();

    left = -1;
}

ParticleSystem::~ParticleSystem() {
    delete emit_shape;
}

void ParticleSystem::set_amount(int amt) {
    amount = amt;
}

void ParticleSystem::spawn_particle() {
    json data = *particle_data.get();

    Particle new_particle;

    // Transform
    float scale_coeff = (scale.x + scale.y)*.5f;
    new_particle.position  = Vector2Add(position, offset);
    new_particle.position  = emit_shape->get_pos(new_particle.position, scale_coeff);

    new_particle.scale     = particle_scale + (particle_scale_randomness*.5 * RandF2());
    new_particle.scale_end = particle_scale_end * new_particle.scale;

    new_particle.scale     *= scale_coeff;
    new_particle.scale_end *= scale_coeff;

    new_particle.angle = RAD2DEG * angle + particle_angle + (particle_angle_randomness*.5 * RandF2());
    new_particle.angular_velocity = angular_velocity  + (angular_velocity_randomness*.5 * RandF2());

    // Color
    int rand_r = particle_tint_randomness*.5 * RandF2(),
        rand_g = particle_tint_randomness*.5 * RandF2(),
        rand_b = particle_tint_randomness*.5 * RandF2();

    new_particle.tint = Color{
        static_cast<unsigned char>(tint.r + rand_r),
        static_cast<unsigned char>(tint.g + rand_g),
        static_cast<unsigned char>(tint.b + rand_b),
        tint.a
    };

    new_particle.tint_end = Color{
        static_cast<unsigned char>(particle_tint_end.r + rand_r),
        static_cast<unsigned char>(particle_tint_end.g + rand_g),
        static_cast<unsigned char>(particle_tint_end.b + rand_b),
        particle_tint_end.a
    };

    // Lifetime
    new_particle.lifetime_max = lifetime + lifetime_randomness*.5f * RandF2();
    new_particle.lifetime     = new_particle.lifetime_max;

    // Velocity
    new_particle.velocity = Vector2Rotate(
        {velocity + velocity_randomness*.5f * RandF2(), 0},
        shot_angle * DEG2RAD + RandF2() * spread*.5f + angle
    );
    new_particle.velocity = Vector2Multiply(new_particle.velocity, {scale_coeff, scale_coeff});

    particles.push_back(new_particle);
}

void ParticleSystem::attempt_particle_bounce(Particle &particle, Vector2 dir, float velocity_anim, float delta) {

    if (collision_mask != nullptr) {
        bool colliding = collision_mask->get_tile(
            round(particle.position.x / collision_mask->tilesize.x),
            round(particle.position.y / collision_mask->tilesize.y)
        ) != -1;

        if (colliding) {
            particle.position.x -= Lerp(particle.velocity.x, particle.velocity.x * velocity_end, velocity_anim) * delta * dir.x * 2;
            particle.position.y -= Lerp(particle.velocity.y, particle.velocity.y * velocity_end, velocity_anim) * delta * dir.y * 2;

            if (dir.x != 0)
                particle.velocity.x *= -dir.x;
            
            if (dir.y != 0)
                particle.velocity.y *= -dir.y;
        }
    }
}

void ParticleSystem::process(float delta) {
    if (TryingToHotReload())
        reload_data();
    
    // Spawn particles when timer runs out
    spawn_timer -= delta;

    if (spawn_timer <= 0 && left != 0) {
        spawn_timer = 1.0 / (firerate + firerate_randomness*.5f * RandF2());

        left--;

        for (int i = 0; i < amount; i++)
            spawn_particle();
    }

    // Loop trough particles and queue dead ones before drawing
    std::vector<int> kill_queue {};

    int i = 0;
    for (auto& particle: particles) {
        // Calculate animation value
        float velocity_anim = Easing::easing_functions[velocity_ease_name](
            1.0f - particle.lifetime / particle.lifetime_max
        );

        // Add force
        particle.velocity = Vector2Add(particle.velocity,
            Vector2Multiply(force, {delta, delta})
        );

        // Add velocity
        particle.position.x += Lerp(particle.velocity.x, particle.velocity.x * velocity_end, velocity_anim) * delta;
        attempt_particle_bounce(particle, Vector2{0.75, 0}, velocity_anim, delta);

        particle.position.y += Lerp(particle.velocity.y, particle.velocity.y * velocity_end, velocity_anim) * delta;
        attempt_particle_bounce(particle, Vector2{0, 0.75}, velocity_anim, delta);

        // Add angular velocity
        particle.angle += particle.angular_velocity * delta;

        if (rotate_to_velocity)
            particle.angle = atan2(particle.velocity.y, particle.velocity.x) * RAD2DEG;

        // Lower lifetime and queue particles deletion if needed
        particle.lifetime -= delta;
        if (particle.lifetime <= 0)
            kill_queue.push_back(i);

        i++;
    }

    // Remove all dead particles
    for (size_t i = 0; i < kill_queue.size(); i++) {
        particles.erase(particles.begin() + kill_queue[kill_queue.size()-1 - i]);
    }
}

int ParticleSystem::get_num_particles() {
    return particles.size();
}

void ParticleSystem::draw() {
    for (auto& particle: particles) {
        // Calculate animation values (0 - 1) based on custom easing functions
        float scale_anim = Easing::easing_functions[scale_ease_name](
            1.0f - particle.lifetime / particle.lifetime_max
        );

        float tint_anim = Easing::easing_functions[tint_ease_name](
            1.0f - particle.lifetime / particle.lifetime_max
        );

        float calc_scale = Lerp(particle.scale, particle.scale_end, scale_anim);
        Color calc_tint = Lerp(particle.tint, particle.tint_end, tint_anim);
        calc_scale = std::max(calc_scale, 0.f);

        DrawTextureCentered(texture.get(),
            particle.position,
            {calc_scale, calc_scale},
            particle.angle,
            calc_tint
        );
    }
}

ParticleEntity::ParticleEntity(std::string path, Vector2 position, int left, Tilemap *mask):
    system {ParticleSystem(path, position)} {

    add_component(new TransformComponent(this, position));
    system.set_left(left);
    
    set_name("ParticleEffect");

    if (mask != nullptr) {
        system.set_collision_mask(mask);
    }
}

void ParticleEntity::process(float delta) {
    system.update_transform((TransformComponent *)get_component(CompType::TRANSFORM));

    if (system.get_num_particles() == 0 && system.get_left() == 0) {
        queue_free();
    }
}