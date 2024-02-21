#include <scene.hpp>
#include <test_entity.hpp>

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
    }
}

void Scene::process(float delta) {}

// Test scene
TestScene::TestScene(): Scene("test_scene") {
    restart();
}

void TestScene::restart() {
    entities = {
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