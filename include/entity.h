#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <map>
#include <raylib.h>
#include <component.h>
#include <unordered_set>

class Entity {
protected:
    std::map<ComponentType, Component*> components;
    std::unordered_set<std::string> groups;

    virtual void process_components(float delta);

public:
    Entity();
 
    virtual void join_group();
    virtual void leave_group();
    virtual bool is_in_group();

    virtual void process(float delta) = 0;
    virtual void draw(float delta) = 0;
};

#endif