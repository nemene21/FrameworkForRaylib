#ifndef SCENE_H
#define SCENE_H

#include <entity.hpp>
#include <vector>

class Scene {
public:
    Scene();

    virtual void process(float delta);

    virtual void draw_entities(float delta);
    virtual void process_entities(float delta);

    virtual void restart() = 0;

protected:
    std::vector<Entity*> entities;
};

class TestScene: public Scene {
public:
    TestScene();
    void restart();
};

#endif