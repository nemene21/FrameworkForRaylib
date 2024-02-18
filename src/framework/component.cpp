#include <component.hpp>


// <Signals>
void Signal::connect(const SignalFunction &function) {
    callers.push_back(function);
}

void Signal::emit(Entity *parent) {

    for (SignalFunction caller: callers) {
        caller(parent);
    }
}

// <Component>
Component::Component(ComponentType type, Entity *entity):
    type {type}, entity {entity}
    {
    ComponentManager::add_component(this);
}

Component::~Component() {
    ComponentManager::remove_component(this);
}

// <Component manager>
ComponentMap ComponentManager::component_map {};

// Component adder/remover
void ComponentManager::add_component(Component *comp) {
    component_map[comp->type].insert(comp);
}

void ComponentManager::remove_component(Component *comp) {
    component_map[comp->type].erase(comp);
}
// Component query
ComponentSet ComponentManager::query_components(ComponentType type) {
    return component_map[type];
}

// Counters
int ComponentManager::component_type_count(ComponentType type) {
    return component_map[type].size();
}

int ComponentManager::component_count() {
    int sum = 0;

    for (auto& keyvalue: component_map) {
        sum += keyvalue.second.size();
    }
    return sum;
}