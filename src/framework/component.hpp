#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <set>
#include <functional>
#include <map>
#include <vector>
#include <set>

class Entity;

typedef std::function<void(Entity *)> SignalFunction;

/// @brief Calls callbacks when a certain event happens
class Signal {
protected:
    std::vector<SignalFunction> callers;

public:
    Signal();
    /// @brief Connect a callback function to the signal
    /// @param func Pointer to callback
    /// @note Callbacks return nothing and have only one parameter (Entity*)
    void connect(SignalFunction func);
    /// @brief Calls all the callbacks (event happened)
    /// @param parent The entity the callbacks referance
    void emit(Entity* parent);
};

/// @brief All component types
enum ComponentType {
    TRANSFORM,
    VELOCITY,
    HEALTH,
    CAMERA,
    ANIMATION,
    COLLIDER,
    AREA,
    TIMER,
    /// @brief Number of components in framework
    COUNT,
};

typedef ComponentType CompType;

/// @brief Component object
/// @note This is not a pure ECS, the components hold both data and functionality
class Component {
public:
    ComponentType type;
    Entity *entity;

    Component(ComponentType type, Entity *entity);
    virtual ~Component();

    /// @brief Adds the component to its entity
    virtual void setup();

    virtual void process(float delta);
    virtual void draw(float delta);
};

// <Component Namespace>
typedef std::set<Component *> ComponentSet;

typedef std::map<ComponentType, ComponentSet>
    ComponentMap;

/// @brief Is responsible for component queries
class ComponentManager {
protected:
    static ComponentMap component_map;

public:
    static void add_component(Component *comp);
    static void remove_component(Component *comp);

    /// @brief Returns a set of components of a certain type
    /// @param type CompType::YOUR_COMPONENT_TYPE
    /// @return Set of components
    static ComponentSet query_components(ComponentType type);
    /// @brief Returns how many components of "type" type there are
    /// @param type CompType::YOUR_COMPONENT_TYPE
    /// @return Number of components
    static int component_type_count(ComponentType type);
    /// @brief Returns the number of all components
    /// @return Number of all components
    static int component_count();
};

#endif