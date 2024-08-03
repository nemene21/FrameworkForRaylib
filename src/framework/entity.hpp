#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <map>
#include <raylib.h>
#include <component.hpp>
#include <unordered_set>
#include <networking/packets.hpp>

class Component;

enum class SyncedEntityType {
    PLAYER,
    COUNT,
};

/// @brief Object in a scene, it has a map of components that are automatically processed and drawn
class Entity {
protected:
    std::map<ComponentType, Component*> comps;
    std::unordered_set<std::string> groups;
    std::string name;

    bool death_queued;

public:
    Entity(std::string name);
    Entity();
    virtual ~Entity();

    bool owned;
    int id;
    EntityType type;
    void network_update_components();

    /// @brief Returns true if the entity is synced over the network
    bool is_synced();

    virtual void process_components(float delta);
    virtual void draw_components(float delta);

    /// @brief Returns array of components on entity
    virtual std::vector<Component*> get_components();

    virtual std::string get_name();
    /// @brief Sets name of entity
    /// @param new_name The new name
    /// @warning Do not rename entities while they are already in a scene
    virtual void set_name(std::string new_name);
    virtual std::unordered_set<std::string>& get_groups();

    /// @brief Makes the entity be in a group
    /// @param group Group name
    /// @note Groups are used to query components of a certain group or to check if an entity should have a certain functionality
    virtual void join_group(std::string group);
    /// @brief Makes the entity leave a group
    /// @param group Group name
    virtual void leave_group(std::string group);
    /// @brief Checks if the entity is in the group
    /// @param group Group name
    /// @retval TRUE Is in the group
    /// @retval FALSE Is not in the group
    virtual bool is_in_group(std::string group);

    /// @brief Deletes the entity
    virtual void queue_free();
    /// @brief Returns true if the entity is queued for deletion
    /// @retval TRUE Is queued for deletion
    /// @retval FALSE Is not queued for deletion
    virtual bool is_death_queued();

    virtual size_t get_size();

    /// @brief Adds the component to the entity
    /// @param comp Component pointer
    virtual void add_component(Component* comp);
    /// @brief Removes the component of a certain type
    /// @param type CompType::YOUR_COMPONENT_TYPE
    virtual void remove_component(ComponentType type);
    /// @brief Returns true if the entity has a certain component
    /// @param type CompType::YOUR_COMPONENT_TYPE
    /// @retval TRUE Entity has a certain component
    /// @retval FALSE Entity doesn't have a certain component
    virtual bool has_component(ComponentType type);
    /// @brief Returns a pointer to the wanted component
    /// @param type CompType::YOUR_COMPONENT_TYPE
    /// @return Pointer to component
    virtual Component *get_component(ComponentType type);

    virtual void process(float delta);
    virtual void draw(float delta);
};

#endif