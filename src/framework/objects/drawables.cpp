#include <drawables.hpp>

// <Texture Manager>
std::map<std::string, TexturePtr> TextureManager::texture_map;
float TextureManager::timer = 0.0f;
float TextureManager::tick  = 100.0f;

// Load a texture and put it's smart pointer into the texture map (it's path is the key)
void TextureManager::load(std::string name) {
    texture_map[name] = std::make_shared<Texture2D>(
        LoadTexture((TEXTURE_DIR name).c_str())
    );
}

// Returns a texture smart pointer and loads the texture if required
TexturePtr TextureManager::get(std::string name) {
    if (texture_map.find(name) != texture_map.end())
        return texture_map[name];

    load(name);
    return texture_map[name];
}

// Unloads a texture
void TextureManager::unload(std::string name) {

    UnloadTexture(*texture_map[name].get());
    texture_map.erase(name);
}

// Unloads all textures that aren't being referanced 
void TextureManager::unload_unused() {
    for (auto& texture_pair: texture_map) {

        if (texture_pair.second.use_count() == 1) {
            unload(texture_pair.first);
        }
    }
}
// Ticks down a timer which calls "unload_unused()" when it hits 0 every "tick" seconds
void TextureManager::unload_check() {
    timer -= GetFrameTime();

    if (timer < .0) {
        timer = tick;

        unload_unused();
    }
}

// Reloads all textures
void TextureManager::reload() {
    for (auto& texture_pair: texture_map) {

        UnloadTexture(*texture_pair.second.get());
        *texture_pair.second.get() = LoadTexture((TEXTURE_DIR texture_pair.first).c_str());
    }
}

// Unloads all textures
void TextureManager::unload_all() {
    for (auto& texture_pair: texture_map) {

        unload(texture_pair.first);
    }
}


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
ShaderBond::ShaderBond(std::string shader_path): shader {ShaderManager::get(shader_path)}, bound_textures {} {}
ShaderBond::ShaderBond(ShaderPtr shader): shader {shader}, bound_textures {} {}

void ShaderBond::use() {
    BeginShaderMode(*shader.get());
    
    for (BoundTexture& bound_texture: bound_textures) {
        SetShaderValueTexture(
            *shader.get(),
            GetShaderLocation(*shader.get(), bound_texture.name.c_str()),
            *bound_texture.texture.get()
        );
    }}

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

void ShaderBond::send_uniform(std::string name, void *ptr, int type) {
    SetShaderValue(*shader.get(), GetShaderLocation(*shader.get(), name.c_str()), ptr, type);
}

void ShaderBond::bind_texture(std::string name, TexturePtr texture) {
    bound_textures.push_back({texture, name});
}

// <Drawables>
Drawable::Drawable(Vector2 position, Vector2 offset, Vector2 scale, float angle, std::string shader_path):
    position {position},
    offset {offset},
    scale {scale},
    angle {angle},
    tint {255, 255, 255, 255},
    z_coord {0},
    shader_bond {ShaderBond(shader_path)} {

        DrawableManager::add(this);
    }

Drawable::~Drawable() {
    DrawableManager::remove(this);
}

void Drawable::update_transform(TransformComponent *trans_comp) {
    position = trans_comp->position;
    scale = trans_comp->scale;
    angle = trans_comp->angle;
}

Vector2 Drawable::real_pos() {
    return Vector2Add(position, offset);
}

// <Drawable Manager>
std::set<Drawable *> DrawableManager::drawables {};

bool drawable_comparison(Drawable *a, Drawable *b) {
    return a->z_coord < b->z_coord;
}

void DrawableManager::draw() {
    std::vector<Drawable *> sorted (drawables.begin(), drawables.end());
    std::sort(sorted.begin(), sorted.end(), drawable_comparison);

    for (auto drawable: sorted) {
        drawable->shader_bond.use();
        drawable->draw();
        EndShaderMode();
    }
}

void DrawableManager::clear() {
    drawables.clear();
}

void DrawableManager::add(Drawable *drawable) {
    drawables.insert(drawable);
}

void DrawableManager::remove(Drawable *drawable) {
    drawables.erase(drawable);
}