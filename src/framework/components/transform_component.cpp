#include <transform_component.hpp>

TransformComponent::TransformComponent(Entity *entity, Vector2 position, Vector2 scale, float angle):
    Component(ComponentType::TRANSFORM, entity),
    position {position},
    scale {scale},
    velocity {0, 0},
    angle {angle}
    {}

void TransformComponent::translate(Vector2 adding) {
    position = Vector2Add(position, adding);
}

void TransformComponent::translate_x(float adding) {
    position.x += adding;
}

void TransformComponent::translate_y(float adding) {
    position.y += adding;
}

void TransformComponent::check_bounds(Vector2 direction) {
    if (entity->has_component(CompType::COLLIDER)) {
        auto collider_ptr = (ColliderComponent *)entity->get_component(CompType::COLLIDER);
        
        if (direction.y == 0)
            collider_ptr->position.x = position.x;
        else
            collider_ptr->position.y = position.y;
        
        collider_ptr->collide(direction);
        position = collider_ptr->position;
    }
}

void TransformComponent::process(float delta) {

    position.x += velocity.x * delta;
    if (abs(velocity.x) > 1) check_bounds({(float)(velocity.x > 0.f) * 2.f - 1.f, 0.f});

    position.y += velocity.y * delta;
    if (abs(velocity.y) > 1) check_bounds({0.f, (float)(velocity.y > 0.f) * 2.f - 1.f});

    if (entity->has_component(CompType::AREA)) {
        auto area_comp = (AreaComponent *)entity->get_component(CompType::AREA);
        area_comp->position = position;
    }
}

void TransformComponent::interpolate_velocity(Vector2 to, float speed) {
    velocity = Lerpi(velocity, to, speed);
}
void TransformComponent::accelerate(Vector2 by) {
    float delta = GetFrameTime();
    velocity = Vector2Add(velocity, Vector2Multiply(by, {delta, delta}));
}