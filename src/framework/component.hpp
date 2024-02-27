#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <set>
#include <functional>
#include <map>
#include <vector>
#include <set>

class Entity;

using SignalFunction = std::function<void(Entity*)>;

// <Signals>
class Signal {
protected:
    std::vector<SignalFunction> callers;

public:
    void connect(const SignalFunction& function);

    void emit(Entity* parent);
};

// <Component Superclass>
enum ComponentType {
    TRANSFORM,
    VELOCITY,
    HEALTH,
    CAMERA,
    COUNT,
};

typedef ComponentType CompType;

class Component {
public:
    const ComponentType type;
    Entity *entity;

    Component(ComponentType type, Entity *entity);
    virtual ~Component();

    virtual void process(float delta);
};

// <Component Namespace>
typedef std::set<Component*> ComponentSet;

typedef std::map<ComponentType, ComponentSet>
    ComponentMap;

class ComponentManager {
protected:
    static ComponentMap component_map;

public:
    static void add_component(Component *comp);

    static void remove_component(Component *comp);

    static ComponentSet query_components(ComponentType type);

    static int component_type_count(ComponentType type);

    static int component_count();
};

#endif