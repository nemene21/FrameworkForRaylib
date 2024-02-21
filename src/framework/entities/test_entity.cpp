#include <test_entity.hpp>

TestEntity::TestEntity():
    sprite {Sprite("test.png")},
    particle_sys {ParticleSystem("test.json")},
    trail_vfx {Trail({0, 0}, 24, 24, RED)}
{

    add_component(
        new TransformComponent(this)
    );
    add_component(
        new HealthComponent(this, 10)
    );
}

void TestEntity::process(float delta) {
    particle_sys.process(delta);
    trail_vfx.process(delta);

    TransformComponent *transform_comp = (TransformComponent*)comps[CompType::TRANSFORM];
    transform_comp->position = GetMousePosition();

    sprite.set_position(transform_comp->position);
    particle_sys.set_position(transform_comp->position);
    trail_vfx.set_position(transform_comp->position);

    sprite.set_scale({
        1.f + (float)sin(GetTime()*PI) * 0.5f,
        1.f + (float)cos(GetTime()*PI) * 0.5f
    });
    sprite.angle += delta * 180;
}

void TestEntity::draw(float delta) {
    trail_vfx.draw();
    particle_sys.draw();
    sprite.draw();
}