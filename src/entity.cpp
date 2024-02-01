#pragma once

#include <iostream>
#include <map>
#include "include/raylib.h"
#include "component.cpp"

class Entity {
protected:
    std::map<ComponentType, Component*> components;

    void process_components(float delta) {
        for (const auto& pair: components) {
            
            Component* component = pair.second;
            component->process(delta);
        }
    }

public:
    Entity(): components {} {}

    virtual void process(float delta) {}
    virtual void draw   (float delta) {}
};