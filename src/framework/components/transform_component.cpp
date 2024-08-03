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

void TransformComponent::network_update() {
    auto packet = TransformUpdatePacket{
        PacketType::COMPONENT_UPDATE,
        true,
        entity->id,
        type,
        position, velocity, scale, angle
    };

    Networking::send(&packet, sizeof(packet), false);
}

void TransformComponent::recieve_update(ComponentUpdatePacket* packet) {
    auto cast_packet = reinterpret_cast<TransformUpdatePacket*>(packet);
    position = cast_packet->position;
    velocity = cast_packet->velocity;
    scale = cast_packet->scale;
    angle = cast_packet->angle;
}

void TransformComponent::draw_gui_info() {
    if (ImGui::CollapsingHeader(("Transform##" + std::to_string(id)).c_str())) {
        ImGui::Indent(25.f);

        float pos[2] = {position.x, position.y};
        ImGui::DragFloat2(("Position##" + std::to_string(id)).c_str(), pos);
        position = {pos[0], pos[1]};

        float vel[2] = {velocity.x, velocity.y};
        ImGui::DragFloat2(("Veclotiy##" + std::to_string(id)).c_str(), vel);
        velocity = {vel[0], vel[1]};

        float scl[2] = {scale.x, scale.y};
        ImGui::DragFloat2(("Scale##" + std::to_string(id)).c_str(), scl, 0.01);
        scale = {scl[0], scl[1]};

        ImGui::DragFloat(("Angle##" + std::to_string(id)).c_str(), &angle);
        
        ImGui::Unindent(25.f);
    }
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
    if (abs(velocity.x) > 0.1f)
        check_bounds({(float)(velocity.x > 0.f) * 2.f - 1.f, 0.f});

    else if(entity->has_component(CompType::COLLIDER))
        ((ColliderComponent*)entity->get_component(CompType::COLLIDER))->position.x = position.x;
    

    position.y += velocity.y * delta;
    if (abs(velocity.y) > 0.1f)
        check_bounds({0.f, (float)(velocity.y > 0.f) * 2.f - 1.f});

    else if (entity->has_component(CompType::COLLIDER))
        ((ColliderComponent*)entity->get_component(CompType::COLLIDER))->position.y = position.y;
    

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