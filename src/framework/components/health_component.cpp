#include <health_component.hpp>

// <Health Component>
HealthComponent::HealthComponent(Entity *entity, float max_hp):
    Component(ComponentType::HEALTH, entity),
    hp {max_hp},
    max_hp {max_hp},
    dead {dead} {}

void HealthComponent::hurt(float damage) {
    hp -= damage;

    hurt_signal.emit(entity);
    check_death();
}

void HealthComponent::heal(float add) {
    hp += add;

    healed_signal.emit(entity);
}

bool HealthComponent::check_death() {
    if (dead)
        return dead;
    
    dead = hp <= 0;

    if (dead)
        died_signal.emit(entity);
    
    return dead;
}