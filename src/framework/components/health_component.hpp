#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include <component.hpp>

class HealthComponent: public Component {
protected:
    float hp, max_hp;

public:
    bool dead;

    HealthComponent(Entity *entity, float max_hp);

    Signal died_signal,
        hurt_signal,
        healed_signal;

    /// @brief Subtracts from hp
    /// @param damage Damage to subtract
    void hurt(float damage);
    /// @brief Adds to hp
    /// @param add Health to add
    void heal(float add);

    /// @brief Returns true if the entity should die
    bool check_death();
};

#endif