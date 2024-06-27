#ifndef SPRITE_H
#define SPRITE_H

#include <drawables.hpp>

/// @brief Draws a centered texture
/// @param texture_ptr Pointer to texture
/// @param position 
/// @param scale Scale vector
/// @param angle 
/// @param tint 
void DrawTextureCentered(Texture2D *texture_ptr, Vector2 position, Vector2 scale={1, 1}, float angle=0, Color tint=WHITE);
/// @brief Draws a centered frame of texture
/// @param texture_ptr Pointer to texture
/// @param frame X and y position of a frame in the texture
/// @param max_frames The number of frames on the x and y axis in the texture
/// @param position 
/// @param scale Scale vector
/// @param angle 
/// @param tint 
void DrawTextureSheet(
    Texture2D *texture_ptr,
    Vector2 frame,
    Vector2 max_frames,
    Vector2 position,
    Vector2 scale={1, 1},
    float angle=0,
    Color tint=WHITE
);

/// @brief Draws a single texture based on it's transform properties
class Sprite: public Drawable {
public:
    Sprite(std::string texture_name, Vector2 position={0,0}, Vector2 scale={1,1}, float angle=0);
    void draw();
    
protected:
    TexturePtr texture;
};

#endif