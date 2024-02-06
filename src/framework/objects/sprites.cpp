#include <sprites.h>

// <Shader Manager>
std::map<std::string, ShaderPtr> ShaderManager::shader_map;
float ShaderManager::timer = 0.0f;
float ShaderManager::tick  = 100.0f;

// Load shader and put it's smart pointer into the shader map (it's path is the key)
void ShaderManager::load(std::string name) {
    shader_map[SHADER_DIR name] = std::make_shared<Texture2D>(
        LoadShader(NULL, name.c_str())
    );
}

// Get shader from shader map by file path (loads the shader if it doesn't exist)
ShaderPtr ShaderManager::get(std::string name) {
    if (shader_map.find(SHADER_DIR name) != shader_map.end())
        return shader_map[SHADER_DIR name];

    load(name);
    return shader_map[SHADER_DIR name];
}

// Unloads shader
void ShaderManager::unload(std::string name) {
    shader_map.erase(SHADER_DIR name);
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

// <Texture Manager>
std::map<std::string, TexturePtr> TextureManager::texture_map;
float TextureManager::timer = 0.0f;
float TextureManager::tick  = 100.0f;

void TextureManager::load(std::string name) {
    texture_map[TEXTURE_DIR  name] = std::make_shared<Texture2D>(
        LoadTexture(name.c_str())
    );
}

TexturePtr TextureManager::get(std::string name) {
    if (texture_map.find(TEXTURE_DIR name) != texture_map.end())
        return texture_map[TEXTURE_DIR name];

    load(name);
    return texture_map[TEXTURE_DIR  name];
}

void TextureManager::unload(std::string name) {
    texture_map.erase(TEXTURE_DIR name);
}

void TextureManager::unload_unused() {
    for (auto& texture_pair: texture_map) {

        if (texture_pair.second.use_count() == 1) {
            unload(texture_pair.first);
        }
    }
}

void TextureManager::unload_check() {
    timer -= GetFrameTime();

    if (timer < .0) {
        timer = tick;

        unload_unused();
    }
}


Sprite::Sprite(
        std::string texture_path,
        Vector2 position,
        Vector2 scale,
        float rotation
    ):
        position {position}, scale {scale}, rotation {rotation}, tint {WHITE}
    {
        texture = TextureManager::get(texture_path);
    }

void Sprite::set_shader(std::string shader_name) {
    shader = ShaderManager::get(shader_name);
}

Vector2 Sprite::get_position() {
    return position;
}
void Sprite::set_position(Vector2 new_pos) {
    position = new_pos;
}
void Sprite::translate(Vector2 adding) {
    position = Vector2Add(position, adding);
}

void Sprite::draw() {
    Texture2D *texture_ptr = texture.get();
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
        rotation,
        tint
    );

    // DrawCircleV(position, 6, RED);
}