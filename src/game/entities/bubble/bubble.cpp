#include <entities/bubble/bubble.hpp>

Bubble::Bubble(Vector2 position, Vector2 velocity, float charge):
    Entity(),
    sprite {Sprite("bubble.png")},
    trans_comp {new TransformComponent(this, position)},
    bounces_left {2}
     {
    trans_comp->velocity = {
        Lerp(velocity.x*.25, velocity.x, 1-charge),
        Lerp(velocity.y*.25, velocity.y, 1-charge)
    };
    trans_comp->scale = {Lerp(0.1f, 1.f, charge), Lerp(0.1f, 1.f, charge)};
    sprite.update_transform(trans_comp);
    add_component(trans_comp);
}

void Bubble::pop() {
    ParticleEntity *sys_entity = new ParticleEntity("pop.json", trans_comp->position);

    float scale_coeff = Lerp(.5f, 1.f, trans_comp->scale.x);
    Vector2 sys_scale = {scale_coeff, scale_coeff};

    ((TransformComponent*)sys_entity->get_component(CompType::TRANSFORM))->scale = sys_scale;
    sys_entity->system.set_amount((int)(Lerp(.2f, 1.f, trans_comp->scale.x) * 16.f));

    SceneManager::scene_on->add_entity(sys_entity);
    queue_free();
}

void Bubble::process(float delta) {
    sprite.update_transform(trans_comp);

    if (trans_comp->position.x < 0 || trans_comp->position.x > res.x) {
        trans_comp->velocity.x *= -1;
        if (--bounces_left < 0) pop();
    }
    if (trans_comp->position.y < 0 || trans_comp->position.y > res.y) {
        trans_comp->velocity.y *= -1;
        if (--bounces_left < 0) pop();
    }
}