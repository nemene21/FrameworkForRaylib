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
    std::vector<std::string> to_unload {};

    for (auto& texture_pair: texture_map) {
        if (texture_pair.second.use_count() == 1) {
            to_unload.push_back(texture_pair.first);
        }
    }
    for (auto to: to_unload) unload(to);
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
    std::vector<std::string> to_unload {};
    for (auto& texture_pair: texture_map) {
        to_unload.push_back(texture_pair.first);
    }
    for (auto to: to_unload) unload(to);
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
    std::vector<std::string> to_unload {};

    for (auto& texture_pair: shader_map) {
        if (texture_pair.second.use_count() == 1) {
            to_unload.push_back(texture_pair.first);
        }
    }
    for (auto to: to_unload) unload(to);
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
    std::vector<std::string> to_unload {};
    for (auto& shader_pair: shader_map) {
        to_unload.push_back(shader_pair.first);
    }
    for (auto to: to_unload) unload(to);
}

// <Materials/ShaderBond>
ShaderBond::ShaderBond(std::string shader_path): bound_textures {} {
    if (shader_path != "-") {
        shader = ShaderManager::get(shader_path);

    } else {
        shader = nullptr;
    }
}
ShaderBond::ShaderBond(ShaderPtr shader): shader {shader}, bound_textures {} {}

void ShaderBond::use() {
    if (shader == nullptr) return;

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

void ShaderBond::send_uniform(std::string name, void *ptr, size_t size, int type) {
    void* new_ptr = malloc(size);
    memcpy(new_ptr, ptr, size);
    pending_uniform_updates.push_back({name, new_ptr, type});
}

void ShaderBond::update_uniforms() {
    for (auto& update: pending_uniform_updates) {
        SetShaderValue(
            *shader.get(),
            GetShaderLocation(*shader.get(), update.name.c_str()),
            update.ptr, update.type
        );
        free(update.ptr);
    }
    pending_uniform_updates.clear();
}

void ShaderBond::bind_texture(std::string name, TexturePtr texture) {
    bound_textures.push_back({texture, name});
}

// <Drawables>
Drawable::Drawable(Vector2 position, Vector2 offset, Vector2 scale, float angle, std::string shader_path, bool is_ui):
    position {position},
    offset {offset},
    scale {scale},
    angle {angle},
    tint {255, 255, 255, 255},
    z_coord {0},
    shader_bond {ShaderBond(shader_path)},
    blend_mode {BLEND_ALPHA},
    is_ui {is_ui} {

        DrawableManager::add(this);
    }

Drawable::~Drawable() {
    DrawableManager::remove(this);
}

void Drawable::process(float delta) {}

void Drawable::update_transform(TransformComponent *trans_comp) {
    position = trans_comp->position;
    scale = trans_comp->scale;
    angle = trans_comp->angle;
}

Vector2 Drawable::real_pos() {
    return Vector2Add(position, Vector2Multiply(offset, scale));
}

// <Drawable Manager>
std::set<Drawable *> DrawableManager::drawables {};
std::set<Drawable *> DrawableManager::ui_drawables {};

bool drawable_comparison(Drawable *a, Drawable *b) {
    return (a->z_coord == b->z_coord) ? (a->position.y < b->position.y) : (a->z_coord < b->z_coord);
}

void DrawableManager::render(std::set<Drawable *>& rendering) {
    std::vector<Drawable *> sorted (rendering.begin(), rendering.end());
    std::sort(sorted.begin(), sorted.end(), drawable_comparison);

    for (auto drawable: sorted) {
        drawable->process(GetFrameTime());
        drawable->shader_bond.update_uniforms();
        drawable->shader_bond.use();
        
        BeginBlendMode(drawable->blend_mode);
        drawable->draw();
        EndBlendMode();

        EndShaderMode();
    }
}

void DrawableManager::draw() {
    render(drawables);
}

void DrawableManager::draw_ui() {
    render(ui_drawables);
}

void DrawableManager::clear() {
    drawables.clear();
}

void DrawableManager::add(Drawable *drawable) {
    if (drawable->is_ui)
        ui_drawables.insert(drawable);
    else
        drawables.insert(drawable);
}

void DrawableManager::remove(Drawable *drawable) {
    if (drawable->is_ui)
        ui_drawables.erase(drawable);
    else
        drawables.erase(drawable);
}