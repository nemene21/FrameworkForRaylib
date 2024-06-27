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

#define TEXTURE_DIR (std::string)("assets/images/") +
#define SHADER_DIR  (std::string)("assets/shaders/") +

typedef std::shared_ptr<Texture2D> TexturePtr;

class TextureManager {
public:
    static std::map<
        std::string,
        TexturePtr
    > texture_map;

    static float timer;
    static float tick;

    /// @brief Load texture into memory
    /// @param path Path to texture
    static void load(std::string path);
    /// @brief Unload texture from memory
    /// @param path Path to texture
    static void unload(std::string path);
    /// @brief Returns smart pointer to texture loaded from "path"
    /// @param path Path to texture
    /// @return Smart pointer to texture
    static TexturePtr get(std::string path);

    static void unload_check();
    static void unload_unused();
    static void unload_all();
    static void reload();
};

typedef std::shared_ptr<Shader> ShaderPtr;

class ShaderManager {
public:
    static std::map<
        std::string,
        ShaderPtr
    > shader_map;

    static float timer;
    static float tick;

    /// @brief Loads a shader into memory
    /// @param path Path to the shader
    static void load(std::string path);
    /// @brief Unoads a shader from memory
    /// @param path Path to the shader
    static void unload(std::string path);
    /// @brief Returns smart pointer to shader loaded from "path"
    /// @param path Path to the shader
    /// @return Smart pointer to shader
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

    typedef struct {
        TexturePtr texture;
        std::string name;
    } BoundTexture;

    std::vector<BoundTexture> bound_textures;

    ShaderBond(std::string shader_path);
    ShaderBond(ShaderPtr shader);
    /// @brief Sets the shader pointer to the one loaded from "shader_path"
    /// @param shader_path Path the shader pointer is loaded by
    virtual void set_shader(std::string shader_path);
    /// @brief Sets the shader poiter to "new_shader"
    /// @param new_shader Pointer to the shader that the bond's shader is being set to
    virtual void set_shader(ShaderPtr new_shader);

    /// @brief Calls BeginShaderMode with the bond's shader
    virtual void use();
    virtual void process(float delta);

    /// @brief 
    /// @param name Name of uniform in shader
    /// @param texture Smart pointer to texture thats being bound
    virtual void bind_texture(std::string name, TexturePtr texture);
    /// @brief Updates an uniform in the bond's shader
    /// @param name Name of uniform in shader
    /// @param ptr Void pointer to the value being sent
    /// @param type Type of uniform (eg. SHADER_UNIFORM_FLOAT)
    virtual void send_uniform(std::string name, void *ptr, int type);

    /// @brief Returns a smart pointer to the bond's shader
    /// @return Smart pointer to the bond's shader
    ShaderPtr get_shader();
};

class Drawable {
public:
    Drawable(Vector2 position={0, 0}, Vector2 offset={0, 0}, Vector2 scale={1, 1}, float angle=0, std::string shader_path="-");
    ~Drawable();

    Vector2 position, offset, scale;
    float angle;
    Color tint;

    float z_coord;
    
    ShaderBond shader_bond;

    virtual void draw() = 0;
    virtual void process(float delta);

    /// @brief Updates position, scale and angle based on a transform component
    /// @param trans_comp Pointer to transform component
    virtual void update_transform(TransformComponent *trans_comp);
    /// @brief Returns the actual position of a Drawable object
    /// @return Position of Drawable (position + offst * scale)
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