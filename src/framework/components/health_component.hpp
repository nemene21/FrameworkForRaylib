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

    void hurt(float damage);

    void heal(float add);

    bool check_death();
};

#endif