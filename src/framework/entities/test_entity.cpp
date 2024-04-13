#include <test_entity.hpp>

void TestEntity::stretch(float anim) {
    sprite.set_scale({
        1.f - anim * .5f,
        1.f + anim * .5f
    });
}

void TestEntity::squash(float anim) {
    sprite.set_scale({
        1.f - cos(PI*anim) * .5f,
        1.f + cos(PI*anim) * .5f
    });
}

void TestEntity::return_state(float anim) {
    sprite.set_scale({
        1.5f - anim * .5f,
        .5f  + anim * .5f
    });
}

void TestEntity::spin(float anim) {
    sprite.angle = Easing::ease_in_out(anim) * 360;
}

void TestEntity::change_color_event(float anim) {
    sprite.tint = Color{(unsigned char)(rand()%100), (unsigned char)(rand()%255), (unsigned char)(rand()%255), 255};
}

void track1(Entity *entity) {
    AudioManager::play_track("1.wav", 0.5);
}
 
void track2(Entity *entity) {
    AudioManager::play_track("2.wav", 0.5);
}

TestEntity::TestEntity():
    sprite {Sprite("test.png")},
    particle_sys {ParticleSystem("test.json")},
    track_bool {false}
{
    std::cout << "test init" << std::endl;
    trail_vfx = Trail({0, 0}, 40, 24, BLUE, {255, 0, 0, 0});

    TransformComponent *transform_comp = new TransformComponent(this);
    transform_comp->position = {500, -500};

    add_component(
        transform_comp
    );

    add_component(
        new HealthComponent(this, 10)
    );
    CameraComponent *camera_comp = new CameraComponent(this);
    add_component(
        camera_comp
    );

    ColliderComponent *collider_comp = new ColliderComponent(this, 64, 88);
    collider_comp->set_mask_bit((int)ColliderIndex::TILEMAP, true);

    add_component(
        collider_comp
    );

    std::cout << "gonna make area!" << std::endl;
    AreaComponent *area_component = new AreaComponent(this, 40);
    std::cout << "did!" << std::endl;
    area_component->set_mask_bit((int)AreaIndex::TEST, true);
    add_component(area_component);
    std::cout << "added it!" << std::endl;
    std::cout << "Gonna connect!" << std::endl;

    area_component->area_entered.connect([](Entity *entity) { track1(entity); });
    area_component->area_exited.connect([](Entity *entity) { track2(entity); });
    std::cout << "Connected!" << std::endl;

    AnimationComponent *anim_comp = new AnimationComponent(this);
    
    anim_comp->make_animation("idle_test", 1, true);
    anim_comp->add_keyframe("idle_test", 0, 0.6, [this](float value) { stretch(value); });
    anim_comp->add_keyframe("idle_test", 0.6, 0.8, [this](float value) { squash(value); });
    anim_comp->add_keyframe("idle_test", 0.8, 1.0, [this](float value) { return_state(value); });

    anim_comp->add_keyframe("idle_test", .0f, 1.f, [this](float value) { spin(value); });

    anim_comp->add_event("idle_test", .6f, [this](float value) { change_color_event(value); });

    anim_comp->play("idle_test");

    add_component(
        anim_comp
    );

    CameraManager::bind_camera(camera_comp->get_camera());

    particle_sys.set_collision_mask(((TestScene*)SceneManager::scene_on)->tiles);
    particle_sys.add_force({0, 1500});
}

void TestEntity::process(float delta) {
    particle_sys.process(delta);
    trail_vfx.process(delta);

    ColliderComponent *collider_comp = (ColliderComponent *)get_component(CompType::COLLIDER);

    TransformComponent *transform_comp = (TransformComponent *)get_component(CompType::TRANSFORM);
    transform_comp->interpolate_velocity({
        (float(IsKeyDown(KEY_D)) - float(IsKeyDown(KEY_A))) * 600.f,
        transform_comp->velocity.y 
    }, 15);

    if (!collider_comp->on_floor())
        transform_comp->accelerate({0, 2500.f * (1.f + 1.5f * (float)(transform_comp->velocity.y > 0))});
    else
        transform_comp->velocity.y = 50;
    
    if (collider_comp->on_ceil()) transform_comp->velocity.y = -.5f * transform_comp->velocity.y;

    CameraComponent *camera = (CameraComponent*)get_component(CompType::CAMERA);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

        int x = round((GetMousePosition().x + camera->position.x + camera->offset.x - res.x*.5) / 96.0),
            y = round((GetMousePosition().y + camera->position.y + camera->offset.y - res.y*.5) / 96.0);
        ((TestScene *)SceneManager::scene_on)->tiles->set_tile(x, y, 1);
        
        ((TestScene *)SceneManager::scene_on)->tiles->build();
    }

    auto area_comp = (AreaComponent *)get_component(CompType::AREA);

    sprite.set_position(transform_comp->position);
    particle_sys.set_position(transform_comp->position);
    trail_vfx.set_position(transform_comp->position);

    if (IsKeyPressed(KEY_SPACE) && collider_comp->on_floor()) {
        transform_comp->velocity.y = -1200;

        CameraComponent *camera = (CameraComponent *)get_component(CompType::CAMERA);
        camera->shake(64, 0.25);
        camera->zoom(1.015, 0.15);
        AudioManager::play_sfx("shoot4.mp3", 1, 1, 0, 0.2);
    }
}

void TestEntity::draw(float delta) {
    trail_vfx.draw();
    particle_sys.draw();
    sprite.draw();
}