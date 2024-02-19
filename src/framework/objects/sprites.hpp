#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include <raymath.h>
#include <map>
#include <iostream>
#include <string>
#include <memory>

#define TEXTURE_DIR (std::string)("assets/images/") +
#define SHADER_DIR  (std::string)("assets/shaders/") +

void DrawTextureCentered(Texture2D* texture_ptr, Vector2 position, Vector2 scale={1, 1}, float angle=0, Color tint=WHITE);

typedef std::shared_ptr<Shader> ShaderPtr;

class ShaderManager {
public:
    static std::map<
        std::string,
        ShaderPtr
    > shader_map;

    static float timer;
    static float tick;

    static void load(std::string path);
    static void unload(std::string path);
    static ShaderPtr get(std::string path);

    static void unload_check();
    static void unload_unused();
    static void unload_all();
    static void reload();

    static void update_uniforms();
};


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

class Sprite {
protected:
    Vector2 position, scale;
    float angle;

    ShaderPtr shader;
    TexturePtr texture;
    Color tint;
    
public:
    Sprite(std::string texture_name, Vector2 position={0,0}, Vector2 scale={1,1}, float angle=0);

    void set_shader(std::string shader_name);

    Vector2 get_position();
    void set_position(Vector2 new_pos);
    void translate(Vector2 adding);

    void draw();
};

#endif