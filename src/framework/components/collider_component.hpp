#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include <component.hpp>
#include <misc.hpp>
#include <camera_component.hpp>
#include <raylib.h>
#include <raymath.h>
#include <set>

#define COLLIDER_CHUNK_SIZE (int)1024
#define DRAW_COLLIDERS false

enum class ColliderIndex {
    TILEMAP,
    COUNT,
};

class ColliderComponent: public Component {
private:
    std::set<int> layers;
    std::set<int> mask;

public:
    Vector2 collision_direction;
    bool is_rectangle, is_circle;
    void *shape;
    Vector2 position;

    ColliderComponent();
    ColliderComponent(Entity *entity, float width, float height);
    ColliderComponent(Entity *entity, float radius);

    void set_layers(std::set<int> new_layers);
    void set_layer(int layer, bool enabled);
    void toggle_layer(int layer);
    void remove_layer(int layer);
    void add_layer(int layer);
    std::set<int>& get_layers();

    void set_mask(std::set<int> new_layers);
    void set_mask_bit(int layer, bool enabled);
    void toggle_mask_bit(int layer);
    void remove_mask_bit(int layer);
    void add_mask_bit(int layer);
    std::set<int>& get_mask();

    bool on_floor();
    bool on_ceil();
    bool on_left_wall();
    bool on_right_wall();
    bool on_wall();

    void collide(Vector2 direction);
    void process(float delta);
    void update_shape_position();
    void debug_draw();
};

bool collides(ColliderComponent *coll1, ColliderComponent *coll2);
void resolve_collision(ColliderComponent *coll1, ColliderComponent *coll2);

class ColliderManager {
public:
    typedef std::vector<ColliderComponent *> ColliderChunk;
    typedef std::map<std::pair<int, int>, ColliderChunk> ColliderLayer;

    static void draw_debug();

    static std::vector<ColliderLayer> collider_layers;
    static void reload_colliders();
    static ColliderChunk get_chunk(ColliderLayer &layer, int x, int y);
    static ColliderChunk get_nearby_colliders(ColliderComponent *comp, int layer);
    static void init();
};

#endif