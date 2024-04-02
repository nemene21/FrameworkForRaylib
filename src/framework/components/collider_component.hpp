#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include <component.hpp>
#include <misc.hpp>
#include <raylib.h>
#include <set>

#define COLLIDER_CHUNK_SIZE 1024

enum class ColliderIndex {
    TILEMAP,
    COUNT,
};

class ColliderComponent: public Component {
private:
    std::set<int> layers;

public:
    bool is_rectangle, is_circle;
    void *shape;
    Vector2 position;

    ColliderComponent(Entity *entity, float width, float height);
    ColliderComponent(Entity *entity, float radius);

    void set_layers(std::set<int> new_layers);
    void set_layer(int layer, bool enabled);
    void toggle_layer(int layer);
    void remove_layer(int layer);
    void add_layer(int layer);
    std::set<int>& get_layers();

    void collide(Vector2 direction);
    void process(float delta);
    void draw(float delta);
};

bool collides(ColliderComponent *coll1, ColliderComponent *coll2);
void resolve_collision(ColliderComponent *coll1, ColliderComponent *coll2);

class ColliderManager {
public:
    typedef std::vector<ColliderComponent *> ColliderChunk;
    typedef std::map<std::pair<int, int>, ColliderChunk> ColliderLayer;

    static std::vector<ColliderLayer> collider_layers;
    static void reload_colliders();
    static ColliderChunk get_nearby_colliders(ColliderComponent *comp);
    static void init();
};

#endif