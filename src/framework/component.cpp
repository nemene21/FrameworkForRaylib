#include <component.hpp>
#include <entity.hpp>

// <Component>
Component::Component(ComponentType type, Entity *entity):
    type {type}, entity {entity}
    {
    id = ComponentManager::add_component(this);
}

Component::~Component() {
    ComponentManager::remove_component(this);
}

void Component::draw_gui_info() {
    ImGui::Text(("Debug info not implemented for component of index " + std::to_string(type)).c_str());
}

void Component::setup() {
    entity->add_component(this);
}

void Component::process(float delta) {}
void Component::draw(float delta) {}

void Component::recieve_update(ComponentUpdatePacket* packet) {}
void Component::network_update() {}

// <Component manager>
ComponentMap ComponentManager::component_map {};

// Component adder/remover
int last_component_id = 0;
int ComponentManager::add_component(Component *comp) {
    component_map[comp->type].insert(comp);
    return last_component_id++;
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