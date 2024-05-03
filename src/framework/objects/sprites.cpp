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

void DrawTextureSheet(
    Texture2D *texture_ptr,
    Vector2 frame,
    Vector2 max_frames,
    Vector2 position,
    Vector2 scale,
    float angle,
    Color tint
) {
    float frame_width  = texture_ptr->width   / max_frames.x,
          frame_height = texture_ptr->height  / max_frames.y;

    float width  = frame_width  * scale.x,
          height = frame_height * scale.y;

    DrawTexturePro(
        *texture_ptr,

        Rectangle{
            frame_width * frame.x, frame_height * frame.y,
            frame_width  * .99f,
            frame_height * .99f
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

// <Sprite Class>

// Sprite class constructor
Sprite::Sprite(
        std::string texture_path,
        Vector2 position,
        Vector2 scale,
        float angle
    ):
        position {position}, scale {scale}, tint {WHITE}, angle {angle}
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

// Scale setter/getter
void Sprite::set_scale(Vector2 new_scale) {
    scale = new_scale;
}
Vector2 Sprite::get_scale() {
    return scale;
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