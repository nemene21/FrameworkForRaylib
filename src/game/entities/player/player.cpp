#include <entities/player/player.hpp>

Player::Player():
    Entity(),
    shot_charge {0},
    move_particles {"move.json"},
    sprite {Sprite("player.png")}
     {
    trans_comp = new TransformComponent(this, {100, 100});
    move_particles.z_coord = -1;

    add_component(trans_comp);
    join_group("player");
}

void Player::shoot(float angle) {
    Vector2 vel = Vector2Rotate({1000, 0}, angle * DEG2RAD);
    Entity* bubble = new Bubble(trans_comp->position, vel, shot_charge);

    ((GameScene*)SceneManager::scene_on)->add_entity(bubble);
    shot_charge = 0;
}

void Player::process(float delta) {
    Vector2 input_dir = InputVectorNormalized(
        "left", "right", "up", "down"
    );
    Vector2 target_vel = Vector2Multiply(input_dir, {200, 200});

    trans_comp->interpolate_velocity(target_vel, 16);
    
    float mouse_angle = Vector2LineAngle({trans_comp->position.x, -trans_comp->position.y}, {mouse_pos().x, -mouse_pos().y}) * RAD2DEG;
    trans_comp->angle = mouse_angle;

    move_particles.update_transform(trans_comp);

    move_particles.set_left(!(input_dir.x == 0 && input_dir.y == 0));

    sprite.update_transform(trans_comp);
    sprite.scale.x = 1 - shot_charge * 0.4f;
    sprite.scale.y = 1 + shot_charge * 0.4f;
    if (shot_charge == 1) {
        sprite.scale.x += sinf(GetTime() * 16) * 0.08f;
        sprite.scale.y += cosf(GetTime() * 16) * 0.08f;
    }

    if (IsPressed("shoot")) {
        shot_charge += delta;
        shot_charge = fminf(shot_charge, 1);
    }
    if (IsJustReleased("shoot")) {
        shoot(mouse_angle);
    }
}