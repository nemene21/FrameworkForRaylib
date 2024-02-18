#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <map>
#include <raylib.h>
#include <component.hpp>
#include <unordered_set>

class Entity {
protected:
    std::map<ComponentType, Component*> components;
    std::unordered_set<std::string> groups;

    virtual void process_components(float delta);

public:
    Entity();
 
    virtual void join_group(std::string group);
    virtual void leave_group(std::string group);
    virtual bool is_in_group(std::string group);

    virtual void add_component(Component &comp);
    virtual void remove_component(ComponentType type);

    virtual void process(float delta) = 0;
    virtual void draw(float delta) = 0;
};

#endif