#include <entity.hpp>

// <Entity>

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
void Entity::add_component(Component &comp) {
    components[comp.type] = &comp;
}

void Entity::remove_component(ComponentType type) {
    components.erase(type);
}
