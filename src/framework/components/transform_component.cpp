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

void TransformComponent::process(float delta) {
    position = Vector2Add(position, Vector2Multiply(velocity, {delta, delta}));
}

void TransformComponent::interpolate_velocity(Vector2 to, float speed) {
    velocity = Lerpi(velocity, to, speed);
}