#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include <raymath.h>
#include <map>
#include <iostream>
#include <string>
#include <memory>

#define IMGDIR (std::string)"assets/images/" +

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
};

class Sprite {
protected:
    Vector2 position, scale;
    float rotation;

    Shader* shader;
    TexturePtr texture;
    Color tint;
    
public:
    Sprite(std::string texture_name, Vector2 position={0,0}, Vector2 scale={1,1}, float rotation=0);

    Vector2 get_position();
    void set_position(Vector2 new_pos);
    void translate(Vector2 adding);

    void draw();
};

#endif