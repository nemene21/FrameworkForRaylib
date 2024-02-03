#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <vector>
#include <functional>
#include <map>

class Entity;

template<typename Type>
using SignalFunction = std::function<void(Type)>;

// <Signals>
template<typename Type>
class Signal {
private:
    std::vector<SignalFunction<Type>> connected;

public:
    void connect(const SignalFunction<Type>& function);

    void emit();
};

// <Component Superclass>
enum class ComponentType {
    TRANSFORM,
    VELOCITY,
    HEALTH,
    COUNT,
};

class Component {
public:
    const Entity *entity;
    const ComponentType type;

    Component(ComponentType type, Entity *entity);

    ~Component();

    virtual void process(float delta);
};

// <Health Component>

class HealthComponent: public Component {
private:
    float hp, max_hp;

public:
    HealthComponent(Entity *entity, float hp, float max_hp);

    const Signal<HealthComponent*> signal_died;

    void hurt(float damage);

    void heal(float add);

    bool is_dead();
};

// <Component Namespace>
namespace Components {
    extern std::map<ComponentType, std::vector<Component*>> component_types;

    void add_component(Component *component);

    void remove_component(Component *component);

    std::vector<Component*> query_components(ComponentType type);

    Component *query_component(ComponentType type, int index);

    int component_type_count(ComponentType type);

    int component_count();
}

#endif