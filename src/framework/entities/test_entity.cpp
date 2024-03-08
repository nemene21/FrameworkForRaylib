#include <test_entity.hpp>

TestEntity::TestEntity():
    sprite {Sprite("test.png")},
    particle_sys {ParticleSystem("test.json")}
{
    trail_vfx = Trail({0, 0}, 40, 24, BLUE, {255, 0, 0, 0});

    add_component(
        new TransformComponent(this)
    );
    add_component(
        new HealthComponent(this, 10)
    );
    CameraComponent *camera_comp = new CameraComponent(this);
    add_component(
        camera_comp
    );

    CameraManager::bind_camera(camera_comp->get_camera());

    particle_sys.set_collision_mask(((TestScene*)SceneManager::scene_on)->tiles);
    particle_sys.add_force({0, 1500});
}

void TestEntity::process(float delta) {
    particle_sys.process(delta);
    trail_vfx.process(delta);

    TransformComponent *transform_comp = (TransformComponent*)get_component(CompType::TRANSFORM);
    transform_comp->interpolate_velocity(
        Vector2Multiply({
            float(IsKeyDown(KEY_D)) - float(IsKeyDown(KEY_A)),
            float(IsKeyDown(KEY_S)) - float(IsKeyDown(KEY_W))
        }, {700, 700}
    ), 15);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        CameraComponent *camera = (CameraComponent*)get_component(CompType::CAMERA);

        int x = round((GetMousePosition().x + camera->position.x + camera->offset.x - res.x*.5) / 96.0),
            y = round((GetMousePosition().y + camera->position.y + camera->offset.y - res.y*.5) / 96.0);
        ((TestScene*)SceneManager::scene_on)->tiles->set_tile(x, y, 1);
        
        if (((TestScene*)SceneManager::scene_on)->tiles->changed)
            ((TestScene*)SceneManager::scene_on)->tiles->build();
    }

    sprite.set_position(transform_comp->position);
    particle_sys.set_position(transform_comp->position);
    trail_vfx.set_position(transform_comp->position);

    sprite.set_scale({
        1.f + (float)sin(GetTime()*PI) * 0.5f,
        1.f + (float)cos(GetTime()*PI) * 0.5f
    });
    sprite.angle += delta * 180;

    if (IsKeyPressed(KEY_SPACE)) {
        CameraComponent *camera = (CameraComponent*)get_component(CompType::CAMERA);
        camera->shake(64, 0.25);
        camera->zoom(1.015, 0.15);
        AudioManager::play_sound("shoot4.mp3", 1, 1, 0, 0.2);
    }
}

void TestEntity::draw(float delta) {
    trail_vfx.draw();
    particle_sys.draw();
    sprite.draw();
}