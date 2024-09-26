#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include <component.hpp>
#include <misc.hpp>
#include <camera_component.hpp>
#include <raylib.h>
#include <raymath.h>
#include <set>

#define COLLIDER_CHUNK_SIZE (int)1024
extern bool DRAW_COLLIDERS;

/// @brief Collision layers
enum class ColliderIndex {
    TILEMAP,
    COUNT,
};

/// @brief Handles AABB collision and crude physics resolutions when combined with a transform component
class ColliderComponent: public Component {
private:
    std::set<int> layers;
    std::set<int> mask;

public:
    Vector2 collision_direction;
    bool draw_debug;
    Rectangle shape;
    Vector2 position;

    ColliderComponent(Vector2 pos={0, 0});
    ColliderComponent(Entity *entity, Vector2 pos, float width, float height);

    void draw_gui_info();
    
    /// @brief Sets the layers
    /// @param new_layers Set of layers
    void set_layers(std::set<int> new_layers);
    /// @brief Sets a single layer
    /// @param layer Layer to set
    /// @param enabled
    void set_layer(int layer, bool enabled);
    /// @brief Toggles a single layer
    /// @param layer Layer to toggle
    void toggle_layer(int layer);
    /// @brief Removes a single layer
    /// @param layer Layer to remove
    void remove_layer(int layer);
    /// @brief Adds a single layer
    /// @param layer Layer to add
    void add_layer(int layer);
    /// @brief Returns the set of layers
    /// @return Set of layers
    std::set<int>& get_layers();

    /// @brief Sets the mask layers
    /// @param new_layers Set of mask layers
    void set_mask(std::set<int> new_layers);
    /// @brief Sets a single mask layer
    /// @param layer Mask layer to set
    /// @param enabled
    void set_mask_bit(int layer, bool enabled);
    /// @brief Toggles a single mask layer
    /// @param layer Mask layer to toggle
    void toggle_mask_bit(int layer);
    /// @brief Removes a single mask layer
    /// @param layer Mask layer to remove
    void remove_mask_bit(int layer);
    /// @brief Adds a single mask layer
    /// @param layer Mask layer to add
    void add_mask_bit(int layer);
    /// @brief Returns the set of mask layers
    /// @return Set of mask layers
    std::set<int>& get_mask();

    /// @brief Returns true if the collider is touching a floor
    bool on_floor();
    /// @brief Returns true if the collider is touching a ceiling
    bool on_ceil();
    /// @brief Returns true if the collider is touching a left wall
    bool on_left_wall();
    /// @brief Returns true if the collider is touching a right wall
    bool on_right_wall();
    /// @brief Returns true if the collider is touching any wall
    bool on_wall();

    void collide(Vector2 direction);
    void process(float delta);
    void update_shape_position();
    void debug_draw();
};

bool collides(ColliderComponent *coll1, ColliderComponent *coll2);
void resolve_collision(ColliderComponent *coll1, ColliderComponent *coll2);

/// @brief Manages collider components by splitting them between chunks
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