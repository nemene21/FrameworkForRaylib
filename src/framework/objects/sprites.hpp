#ifndef SPRITE_H
#define SPRITE_H

#include <drawables.hpp>

void DrawTextureCentered(Texture2D *texture_ptr, Vector2 position, Vector2 scale={1, 1}, float angle=0, Color tint=WHITE);
void DrawTextureSheet(
    Texture2D *texture_ptr,
    Vector2 frame,
    Vector2 max_frames,
    Vector2 position,
    Vector2 scale={1, 1},
    float angle=0,
    Color tint=WHITE
);

class Sprite: public Drawable {
public:
    Sprite(std::string texture_name, Vector2 position={0,0}, Vector2 scale={1,1}, float angle=0);

    Vector2 get_position();
    void set_position(Vector2 new_pos);
    void translate(Vector2 adding);

    Vector2 get_scale();
    void set_scale(Vector2 new_scale);

    void draw();
    
protected:
    TexturePtr texture;
};

#endif