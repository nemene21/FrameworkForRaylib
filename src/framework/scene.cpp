#include <scene.hpp>
#include <test_entity.hpp>

Scene::Scene(): entities {} {}

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
TestScene::TestScene(): Scene() {
    restart();
}


void TestScene::restart() {
    std::cout << "Entity initializations" << std::endl;
    
    entities = {
        (Entity*)(new TestEntity())
    };
}