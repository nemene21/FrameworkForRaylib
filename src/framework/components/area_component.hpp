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

enum class AreaIndex {
    TEST,
    COUNT,
};

class AreaComponent: public Component {
private:
    std::set<int> layers;
    std::set<int> mask;

public:
    bool is_rectangle, is_circle;
    void *shape;
    Vector2 position;
    std::set<AreaComponent *> areas_overlapping;

    AreaComponent();
    AreaComponent(Entity *entity, float width, float height);
    AreaComponent(Entity *entity, float radius);

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

    void check_overlaps();

    void process(float delta);
    void update_shape_position();
    void debug_draw();
};

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