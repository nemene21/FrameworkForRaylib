#include <sprites.hpp>

// Draws a centered texture, you should basically always use this when drawing a texture
void DrawTextureCentered(Texture2D* texture_ptr, Vector2 position, Vector2 scale, float angle, Color tint) {
    Rectangle source_rect = Rectangle{
        0, 0,
        (float)texture_ptr->width,
        (float)texture_ptr->height
    };

    if (scale.x < 0) {
        scale.x = abs(scale.x);
        source_rect.width = -(float)texture_ptr->width;
    }
    if (scale.y < 0) {
        scale.y = abs(scale.y);
        source_rect.height = -(float)texture_ptr->height;
    }
    
    float width  = texture_ptr->width  * scale.x,
          height = texture_ptr->height * scale.y;

    DrawTexturePro(
        *texture_ptr,

        source_rect,

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
// <Sprite Class>

// Sprite class constructor
Sprite::Sprite(
        std::string texture_path,
        Vector2 position,
        Vector2 scale,
        float angle
    ):
        Drawable(position, {0, 0}, scale, angle)
    {
        texture = TextureManager::get(texture_path);
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
    DrawTextureCentered(texture.get(), real_pos(), scale, angle, tint);
}