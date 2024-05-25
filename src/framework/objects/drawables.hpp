#ifndef DRAWABLES_H
#define DRAWABLES_H

#include <raylib.h>
#include <raymath.h>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <transform_component.hpp>

#define SHADER_DIR  (std::string)("assets/shaders/") +

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

class ShaderBond {
public:
    ShaderPtr shader;

    ShaderBond(std::string shader_path);
    ShaderBond(ShaderPtr shader);

    virtual void set_shader(std::string shader_path);
    virtual void set_shader(ShaderPtr new_shader);

    virtual void use();
    virtual void process(float delta);

    ShaderPtr get_shader();
};

class Drawable {
public:
    Drawable(Vector2 position={0, 0}, Vector2 offset={0, 0}, Vector2 scale={1, 1}, float angle=0, std::string shader_path="default.glsl");
    ~Drawable();

    Vector2 position, offset, scale;
    float angle;
    Color tint;

    float z_coord;

    ShaderBond shader_bond;

    virtual void draw() = 0;
    virtual void update_transform(TransformComponent *trans_comp);
    virtual Vector2 real_pos();
};

class DrawableManager {
public:
    static std::set<Drawable *> drawables;

    static void draw();
    static void clear();
    static void add(Drawable *drawable);
    static void remove(Drawable *drawable);
};

#endif