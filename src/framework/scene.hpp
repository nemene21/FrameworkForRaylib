#ifndef SCENE_H
#define SCENE_H

#include <entity.hpp>
#include <vector>

#include <tilemap_entity.hpp>
#include <misc.hpp>

/// @brief Holds entities which are drawn and processed, offers group queries for entities and automatically removes entities queued for deletion
/// @note To use a scene you have to initialise the scene with "new YourSceneClass()" and than set the scene with "SceneManager::set_scene("your_scene_name")"
class Scene {
public:
    /// @brief Used for setting the scene
    std::string name;

    /// @brief Initialises the entity array and automatically sets up the scene in the scene manager
    /// @param name Name of scene
    Scene(std::string name);

    virtual void process(float delta);

    virtual void draw_entities(float delta);
    virtual void process_entities(float delta);

    /// @brief Adds a new entity to the scene
    /// @param entity Pointer to entity, the scene is now responsible for it
    virtual void add_entity(Entity* entity);
    /// @brief Returns the number of entities in the scene
    /// @return Entities in the scene
    virtual int  entity_count();

    /// @brief Returns an array of entities in group
    /// @param name Name of group
    virtual std::vector<Entity*> query_in_group(std::string name);
    /// @brief Returns an entity in the group
    /// @param name Returns an entity in the group
    virtual Entity *first_in_group(std::string name);

    /// @brief Unloads all entities from memory
    virtual void unload_entities();

    virtual void restart() = 0;

protected:
    std::vector<Entity*> entities;
};

typedef std::map<std::string, Scene*> SceneMap;

/// @brief Class responsible for loading and unloading scenes
class SceneManager {
public:
    static SceneMap scene_map;
    static Scene* scene_on;

    /// @brief Sets up a scene (done automatically in the Scene::Scene(name) constructor)
    /// @param scene Pointer to scene
    static void setup_scene(Scene* scene);
    static void unload(std::string name);
    static void unload_all();

    /// @brief Sets the current scene to another one
    /// @param name Scene name
    static void set_scene(std::string name);
};

#endif