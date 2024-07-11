#include <scene.hpp>

// <Scene>
Scene::Scene(std::string name): name {name}, entities {} {
    SceneManager::setup_scene(this);
}

int Scene::entity_count() {
    return entities.size();
}

void Scene::process_entities(float delta) {
    int i = 0;
    while (i != entities.size()) {
        Entity *entity = entities[i];
        entity->process(delta);
        entity->process_components(delta);
        i++;
    }

    // Remove dead entities
    for (int i = entities.size()-1; i >= 0; i--) {

        if (entities[i]->is_death_queued()) {
            entities_by_name.erase(entities[i]->get_name());
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

void Scene::add_entity(Entity* entity) {
    std::string original_name = entity->get_name();
    int i = 2;
    while (entities_by_name.find(entity->get_name()) != entities_by_name.end()) {
        entity->set_name(original_name + "_" + std::to_string(i));
        i++;
    }
    entities_by_name[entity->get_name()] = entity;
    entities.push_back(entity);
}

std::vector<Entity*> Scene::query_in_group(std::string name) {
    std::vector<Entity*> found {};

    for (auto entity: entities) {
        if (entity->is_in_group(name)) found.push_back(entity);
    }
    return found;
}

Entity *Scene::first_in_group(std::string name) {
    for (auto entity: entities) {
        if (entity->is_in_group(name)) return entity;
    }
    return nullptr;
}

void Scene::unload_entities() {
    for (int i = entities.size()-1; i >= 0; i--) {
        entities_by_name.erase(entities[i]->get_name());

        delete entities[i];
        entities.erase(entities.begin() + i);
    }
}

const std::vector<Entity*>& Scene::get_entities() {
    return entities;
}

void Scene::process(float delta) {}

// <Scene Manager>
SceneMap SceneManager::scene_map = {};
Scene*   SceneManager::scene_on = nullptr;

void SceneManager::setup_scene(Scene* scene) {
    scene_map[scene->name] = scene;
}

void SceneManager::set_scene(std::string name) {
    scene_on = scene_map[name];
    scene_map[name]->unload_entities();
    scene_map[name]->restart();
}

void SceneManager::unload(std::string name) {
    delete scene_map[name];
    scene_map.erase(name);
}

void SceneManager::unload_all() {
    std::vector<std::string> to_unload {};
    for (auto& scene_pair: scene_map) {
        to_unload.push_back(scene_pair.first);
    }
    for (auto to: to_unload) unload(to);
}