#ifndef DRAWABLES_H
#define DRAWABLES_H

#include <raylib.h>
#include <raymath.h>
#include <map>
#include <iostream>
#include <string>
#include <memory>

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

#endif