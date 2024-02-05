#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <map>
#include <raylib.h>
#include <component.h>

class Entity {
protected:
    std::map<ComponentType, Component*> components;

    void process_components(float delta);

public:
    Entity();

    virtual void process(float delta);
    virtual void draw(float delta);
};

#endif