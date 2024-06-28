#include <entities/player.hpp>

Player::Player():
    sprite {Sprite("test.png")}
    
     {
    add_component(new TransformComponent(
        this, {100, 100}
    ));
}

void Player::process(float delta) {
    auto *trans_comp = (TransformComponent *)get_component(ComponentType::TRANSFORM);
    sprite.update_transform(trans_comp);

    Vector2 input_dir = InputVectorNormalized("left", "right", "up", "down");
    trans_comp->interpolate_velocity(
        Vector2Multiply(input_dir, {100, 100}),
        20
    );

    sprite.angle = 1 + (trans_comp->velocity.x / 100.f) * 20;
}