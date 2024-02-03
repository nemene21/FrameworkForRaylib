#include <iostream>
#include <vector>
#include <functional>
#include <map>

template<typename Type>
using SignalFunction = std::function<void(Type)>;

enum class ComponentType;
class Component;

// <Component Namespace Declaration>
namespace Components {
    extern std::map<ComponentType, std::vector<Component*>> component_types;

    void add_component(Component *component);

    void remove_component(Component *component);

    std::vector<Component*> query_components(ComponentType type);

    Component *query_component(ComponentType type, int index);

    int component_type_count(ComponentType type);

    int component_count();
}

// <Signals>
template<typename Type>
class Signal {
private:
    std::vector<SignalFunction<Type>> connected;

public:
    void connect(const SignalFunction<Type>& function) {
        connected.push_back(function);
    }

    void emit() {
        for (const auto& call: connected) {
            call();
        }
    }
};

// <Component Superclass>
enum class ComponentType {
    TRANSFORM,
    VELOCITY,
    HEALTH,
    COUNT,
};

class Entity {};

class Component {
public:
    const Entity *entity;
    const ComponentType type;

    Component(ComponentType type, Entity *entity): type {type}, entity {entity} {
        Components::add_component(this);
    };

    ~Component() {
        Components::remove_component(this);
    }

    virtual void process(float delta) {}
};

// <Health Component>

class HealthComponent: public Component {
private:
    float hp, max_hp;

public:
    HealthComponent(Entity *entity, float hp, float max_hp):
        Component(ComponentType::HEALTH, entity),
        hp {hp},
        max_hp {max_hp} {}

    const Signal<HealthComponent*> signal_died;

    void hurt(float damage) { 
        hp -= damage;
    } 

    void heal(float add) {
        hp = std::min(hp + add, max_hp);
    }

    bool is_dead() {
        return hp <= 0;
    }
};

// <Component Namespace>
namespace Components {
    std::map<ComponentType, std::vector<Component*>> component_types {};

    void add_component(Component *component) {

        if (component_types.find(component->type) != component_types.end()) {
            component_types[component->type].push_back(component);
        }
        else {
            component_types[component->type] = std::vector<Component*>{component};
        }
    }

    void remove_component(Component *component) {
        std::vector<Component*>& components = component_types[component->type];

        for (int i = 0; i < components.size(); i++) {
            if (components[i] == component) {
                components.erase(components.begin() + i);
                break;
            }
        }
        std::cout << "Component not found in the global component type map (Womp womp)" << std::endl;
    }

    std::vector<Component*> query_components(ComponentType type) {
        return component_types[type];
    }

    Component *query_component(ComponentType type, int index) {
        return component_types[type][index];
    }

    int component_type_count(ComponentType type) {
        return component_types[type].size();
    }

    int component_count() {
        int sum {0};

        for (auto& list: component_types) {
            sum += list.second.size();
        }
        return sum;
    }
}