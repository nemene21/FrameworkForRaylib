#include <sprites.h>

std::map<std::string, TexturePtr> TextureManager::texture_map;
float TextureManager::timer = 0.0f;
float TextureManager::tick  = 100.0f;

void TextureManager::load(std::string name) {
    texture_map[name] = std::make_shared<Texture2D>(
        LoadTexture(name.c_str())
    );
}

TexturePtr TextureManager::get(std::string name) {
    if (texture_map.find(name) != texture_map.end())
        return texture_map[name];

    load(name);
    return texture_map[name];
}

void TextureManager::unload(std::string name) {
    texture_map.erase(name);
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