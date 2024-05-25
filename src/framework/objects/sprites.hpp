#ifndef SPRITE_H
#define SPRITE_H

#include <drawables.hpp>

#define TEXTURE_DIR (std::string)("assets/images/") +

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


typedef std::shared_ptr<Texture2D> TexturePtr;

class TextureManager {
public:
    static std::map<
        std::string,
        TexturePtr
    > texture_map;

    static float timer;
    static float tick;

    static void load(std::string path);
    static void unload(std::string path);
    static TexturePtr get(std::string path);

    static void unload_check();
    static void unload_unused();
    static void unload_all();
    static void reload();
};

class Sprite: public Drawable {
public:

    Sprite(std::string texture_name, Vector2 position={0,0}, Vector2 scale={1,1}, float angle=0);

    void set_shader(std::string shader_name);

    Vector2 get_position();
    void set_position(Vector2 new_pos);
    void translate(Vector2 adding);

    Vector2 get_scale();
    void set_scale(Vector2 new_scale);

    void draw();
    
protected:
    ShaderPtr shader;
    TexturePtr texture;
};

#endif