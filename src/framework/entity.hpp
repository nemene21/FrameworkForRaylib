#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <map>
#include <raylib.h>
#include <component.hpp>
#include <unordered_set>

class Component;

class Entity {
protected:
    std::map<ComponentType, Component*> comps;
    std::unordered_set<std::string> groups;

    bool death_queued;

public:
    Entity();
    virtual ~Entity();

    virtual void process_components(float delta);
    virtual void draw_components(float delta);
 
    virtual void join_group(std::string group);
    virtual void leave_group(std::string group);
    virtual bool is_in_group(std::string group);

    virtual void queue_free();
    virtual bool is_death_queued();

    virtual void add_component(Component* comp);
    virtual void remove_component(ComponentType type);
    virtual bool has_component(ComponentType type);
    virtual Component *get_component(ComponentType type);

    virtual void process(float delta);
    virtual void draw(float delta);
};

#endif