#include <scene.hpp>

// <Scene>
Scene::Scene(std::string name): name {name}, entities {} {
    SceneManager::setup_scene(this);
}

void Scene::process_entities(float delta) {
    
    for (Entity *entity: entities) {
        entity->process(delta); 
        entity->process_components(delta);
    }

    // Remove dead entities
    for (int i = entities.size()-1; i >= 0; i--) {

        if (entities[i]->is_death_queued()) {
            delete entities[i];
            entities.erase(entities.begin() + i);
        }
    }
}

void Scene::draw_entities(float delta) {

    for (Entity *entity: entities) {
        entity->draw(delta);
        entity->draw_components(delta);
    }
}

void Scene::process(float delta) {}

// Test scene
TestScene::TestScene(): Scene("test_scene") {}

void TestScene::restart() {
    tiles = new Tilemap({96, 96}, "tiles.png");
    FastNoiseLite gen_noise = FastNoiseLite();

    for (int x = 0; x < 128; x++) {
        for (int y = 0; y < 128; y++) {
            if (gen_noise.GetNoise((float)x * 8.f, (float)y * 8.f) > 0.f) tiles->set_tile(x, y, 0);
        }
    }

    tiles->load("test.json");

    entities = {
        (Entity*)tiles,
        (Entity*)(new TestEntity())
    };
}

// <Scene Manager>
SceneMap SceneManager::scene_map = {};
Scene*   SceneManager::scene_on = nullptr;

void SceneManager::setup_scene(Scene* scene) {
    scene_map[scene->name] = scene;
}

void SceneManager::set_scene(std::string name) {
    scene_on = scene_map[name];
    scene_map[name]->restart();
}

void SceneManager::unload(std::string name) {
    delete scene_map[name];
    scene_map.erase(name);
}

void SceneManager::unload_all() {
    for (auto& scene_pair: scene_map) {
        unload(scene_pair.first);
    }
}