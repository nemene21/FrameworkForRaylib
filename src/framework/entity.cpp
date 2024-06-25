#include <entity.hpp>

// <Entity>
Entity::Entity(): death_queued {false} {}
Entity::~Entity() {
    for (auto& comp_pair: comps) {
        delete comp_pair.second;
    }
}

void Entity::process(float delta) {}
void Entity::draw(float delta) {}

void Entity::process_components(float delta) {

    for (auto& comp_pair: comps) {
        comp_pair.second->process(delta);
    }
}

void Entity::draw_components(float delta) {

    for (auto& comp_pair: comps) {
        comp_pair.second->draw(delta);
    }
}

void Entity::queue_free() {
    death_queued = true;
}

bool Entity::is_death_queued() {
    return death_queued;
}

// Groups
void Entity::join_group(std::string group) {
    groups.insert(group);
}

void Entity::leave_group(std::string group) {
    groups.erase(group);
}

bool Entity::is_in_group(std::string group) {
    return groups.find(group) != groups.end();
}

// Components
void Entity::add_component(Component *comp) {
    comps[comp->type] = comp;
}

void Entity::remove_component(ComponentType type) {
    comps.erase(type);
}

bool Entity::has_component(ComponentType type) {
    return (comps.find(type) != comps.end());
}

Component *Entity::get_component(ComponentType type) {
    return comps[type];
}