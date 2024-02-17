#include <sprites.hpp>

// Draws a centered texture, you should basically always use this when drawing a texture
void DrawTextureCentered(Texture2D* texture_ptr, Vector2 position, Vector2 scale, float angle, Color tint) {
    float width  = texture_ptr->width  * scale.x,
        height = texture_ptr->height * scale.y;

    DrawTexturePro(
        *texture_ptr,

        Rectangle{
            0, 0,
            (float)texture_ptr->width,
            (float)texture_ptr->height
        },

        Rectangle{
            position.x, position.y,
            width,
            height
        },
            
        Vector2{width * .5f, height * .5f},
        angle,
        tint
    );
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

// Unloads shader
void ShaderManager::unload(std::string name) {
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
        *shader_pair.second.get() = LoadShader(NULL, (SHADER_DIR shader_pair.first).c_str());
    }
}

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
        *texture_pair.second.get() = LoadTexture((TEXTURE_DIR texture_pair.first).c_str());
    }
}
// <Sprite Class>

// Sprite class constructor
Sprite::Sprite(
        std::string texture_path,
        Vector2 position,
        Vector2 scale,
        float angle
    ):
        position {position}, scale {scale}, angle {angle}, tint {WHITE}
    {
        texture = TextureManager::get(texture_path);
    }

// Shader setter
void Sprite::set_shader(std::string shader_name) {
    shader = ShaderManager::get(shader_name);
}

// Position getter
Vector2 Sprite::get_position() {
    return position;
}
// Position setter
void Sprite::set_position(Vector2 new_pos) {
    position = new_pos;
}
// Position adder
void Sprite::translate(Vector2 adding) {
    position = Vector2Add(position, adding);
}
// Draws the transformed texture with the shader (if it has one)
void Sprite::draw() {

    Shader* shader_ptr = shader.get();
    if (shader_ptr != nullptr)
        BeginShaderMode(*shader_ptr);

    DrawTextureCentered(texture.get(), position, scale, angle, tint);

    if (shader_ptr != nullptr)
        EndShaderMode();

    // DrawCircleV(position, 6, RED);
}