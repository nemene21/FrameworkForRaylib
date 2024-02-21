#include <transform_component.hpp>

TransformComponent::TransformComponent(Entity *entity, Vector2 position, Vector2 scale, float angle):
    Component(ComponentType::TRANSFORM, entity),
    position {position},
    scale {scale},
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