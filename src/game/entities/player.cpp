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
    trans_comp->position = mouse_pos();

    sprite.update_transform(trans_comp);
}