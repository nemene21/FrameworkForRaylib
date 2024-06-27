#ifndef AREA_COMPONENT_H
#define AREA_COMPONENT_H

#include <component.hpp>
#include <misc.hpp>
#include <camera_component.hpp>
#include <raylib.h>
#include <raymath.h>
#include <set>

#define AREA_CHUNK_SIZE (int)1024
#define DRAW_AREAS true

/// @brief Area layers
enum class AreaIndex {
    COUNT,
};

class AreaComponent: public Component {
private:
    /// @brief Layers the area can be detected in
    std::set<int> layers;
    /// @brief Layers the area checks
    std::set<int> mask;

public:
    bool is_rectangle, is_circle;
    void *shape;
    Vector2 position;
    std::set<AreaComponent *> areas_overlapping;

    /// @brief Signal emitted when an area enters area
    Signal area_entered;
    /// @brief Signal emitted when an area exits area
    Signal area_exited;
    AreaComponent *last_entered, *last_exited;

    AreaComponent();
    AreaComponent(Entity *entity, float width, float height);
    AreaComponent(Entity *entity, float radius);

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

    void check_overlaps();
    void clear_overlap();
    /// @brief Returns all overlapping areas
    /// @return Set of overlapping areas
    std::set<AreaComponent *> get_overlapping();
    /// @brief Returns an overlapping area
    /// @return Overlapping area pointer
    AreaComponent *get_one_overlapping();

    void process(float delta);
    void update_shape_position();
    void debug_draw();
};

/// @brief Checks if two areas overlap
/// @param area1 Area pointer
/// @param area2 Area pointer
bool overlaps(AreaComponent *area1, AreaComponent *area2);

class AreaManager {
public:
    typedef std::vector<AreaComponent *> AreaChunk;
    typedef std::map<std::pair<int, int>, AreaChunk> AreaLayer;

    static void draw_debug();

    static std::vector<AreaLayer> area_layers;
    static void reload_areas();
    static AreaChunk get_chunk(AreaLayer &layer, int x, int y);
    static AreaChunk get_nearby_areas(AreaComponent *comp, int layer);
    static void init();
};

#endif