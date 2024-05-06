#include <drawables.hpp>

// <Shader Manager>
std::map<std::string, ShaderPtr> ShaderManager::shader_map;
float ShaderManager::timer = 0.0f;
float ShaderManager::tick  = 100.0f;

// Load shader and put it's smart pointer into the shader map (it's path is the key)
void ShaderManager::load(std::string name) {
    shader_map[name] = std::make_shared<Shader>(
        LoadShader(NULL, (SHADER_DIR name).c_str())
    );
}

// Get shader from shader map by file path (loads the shader if it doesn't exist)
ShaderPtr ShaderManager::get(std::string name) {
    if (shader_map.find(name) != shader_map.end())
        return shader_map[name];

    load(name);
    return shader_map[name];
}

// Unloads shade
void ShaderManager::unload(std::string name) {
    UnloadShader(*shader_map[name].get());

    shader_map.erase(name);
} 

// Unloads all shaders which aren't being referanced
void ShaderManager::unload_unused() {
    for (auto& texture_pair: shader_map) {

        if (texture_pair.second.use_count() == 1) {
            unload(texture_pair.first);
        }
    }
}

// Ticks down timer which will call "unload_unused()" after "tick" seconds
void ShaderManager::unload_check() {
    timer -= GetFrameTime();

    if (timer < .0) {
        timer = tick;

        unload_unused();
    }
}

// Updates uniforms global for all shaders (like time)
void ShaderManager::update_uniforms() {
    float time = GetTime();

    for (auto& shader_pair: shader_map) {

        ShaderPtr shader = shader_pair.second;
        Shader* shader_ptr = shader.get();

        SetShaderValue(
            *shader_ptr,
            GetShaderLocation(*shader_ptr, "time"),
            &time, SHADER_UNIFORM_FLOAT
        );
    }
}

// Reloads all shaders
void ShaderManager::reload() {
    for (auto& shader_pair: shader_map) {

        UnloadShader(*shader_pair.second.get());
        *shader_pair.second.get() = LoadShader(NULL, (SHADER_DIR shader_pair.first).c_str());
    }
}

// Unloads all shader objects
void ShaderManager::unload_all() {
    for (auto& shader_pair: shader_map) {

        unload(shader_pair.first);
    }
}

// <Materials/ShaderBond>
ShaderBond::ShaderBond(std::string shader_path): shader {ShaderManager::get(shader_path)} {}
ShaderBond::ShaderBond(ShaderPtr shader): shader {shader} {}

void ShaderBond::use() {
    BeginShaderMode(*shader.get());
}

void ShaderBond::process(float delta) {}

// Shader setter/getter
void ShaderBond::set_shader(std::string shader_path) {
    shader = ShaderManager::get(shader_path);
}
void ShaderBond::set_shader(ShaderPtr new_shader) {
    shader = new_shader;
}

ShaderPtr ShaderBond::get_shader() {
    return shader;
}

// <Drawables>
Drawable::Drawable(Vector2 position, Vector2 offset, Vector2 scale, float angle, std::string shader_path):
    position {position},
    offset {offset},
    scale {scale},
    angle {angle},
    shader_bond {ShaderBond(shader_path)} {}

void Drawable::update_transform(TransformComponent *trans_comp) {
    position = trans_comp->position;
    scale = trans_comp->scale;
    angle = trans_comp->angle;
}

Vector2 Drawable::real_position() {
    return Vector2Add(position, offset);
}