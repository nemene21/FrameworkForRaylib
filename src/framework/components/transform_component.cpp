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
    std::cout << "bound check!!!" << std::endl;
    if (entity->has_component(CompType::COLLIDER)) {
        auto collider_ptr = (ColliderComponent *)entity->get_component(CompType::COLLIDER);

        collider_ptr->position = position;

        collider_ptr->collide(direction);
    }
}

void TransformComponent::process(float delta) {
    std::cout << "Gonna move!!!" << std::endl;
    position.x += velocity.x * delta;
    std::cout << "Just gotta check bounds now!!!" << std::endl;
    if (velocity.x != 0) check_bounds({(float)(velocity.x > 0.f) * 2.f - 1.f, 0.f});

    position.y += velocity.y * delta;
    if (velocity.y != 0) check_bounds({0.f, (float)(velocity.y > 0.f) * 2.f - 1.f});
}

void TransformComponent::interpolate_velocity(Vector2 to, float speed) {
    velocity = Lerpi(velocity, to, speed);
}