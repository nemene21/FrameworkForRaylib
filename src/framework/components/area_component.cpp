#include <area_component.hpp>

AreaComponent::AreaComponent(): Component(CompType::AREA, nullptr) {}

bool overlaps(AreaComponent *area1, AreaComponent *area2) {
    if (area1->is_rectangle && area2->is_rectangle) {
        Rectangle shape1 = *(Rectangle *)area1->shape;
        Rectangle shape2 = *(Rectangle *)area2->shape;
        return CheckCollisionRecs(
            {shape1.x - shape1.width*.5f, shape1.y - shape1.height*.5f, shape1.width, shape1.height},
            {shape2.x - shape2.width*.5f, shape2.y - shape2.height*.5f, shape2.width, shape2.height}
        );
    }
    if (area1->is_rectangle && area2->is_circle) {
        Circle *circle = (Circle *)area2->shape;
        Rectangle rect = *(Rectangle *)area1->shape;

        return CheckCollisionCircleRec({circle->x, circle->y}, circle->radius, {
            rect.x - rect.width*.5f, rect.y - rect.height*.5f, rect.width, rect.height
        });
    }
    if (area2->is_rectangle && area1->is_circle) {
        Circle *circle = (Circle *)area1->shape;
        Rectangle rect = *(Rectangle *)area2->shape;
        
        return CheckCollisionCircleRec({circle->x, circle->y}, circle->radius, {
            rect.x - rect.width*.5f, rect.y - rect.height*.5f, rect.width, rect.height
        });
    }
    Circle *circle1 = (Circle *)area1->shape;
    Circle *circle2 = (Circle *)area2->shape;

    return CheckCollisionCircles({circle1->x, circle1->y}, circle1->radius, {circle2->x, circle2->y}, circle2->radius);
}

// Area rectangle init
AreaComponent::AreaComponent(Entity *entity, float width, float height):
    Component(CompType::AREA, entity),
    shape {nullptr},
    is_rectangle {true},
    is_circle {false},
    areas_overlapping {}
{
    shape = (void *)(new Rectangle{0, 0, width, height});
}

// Area circle init (crashes due to unimplemented circles)
AreaComponent::AreaComponent(Entity *entity, float radius):
    Component(CompType::AREA, entity),
    shape {nullptr},
    is_rectangle {false},
    is_circle {true},
    areas_overlapping {}
{
    shape = (void *)(new Circle{0, 0, radius});
}

// Area layer manipulation
void AreaComponent::set_layer(int layer, bool enabled) {
    if (enabled)
        layers.insert(layer);
    else
        layers.erase(layer);
}

void AreaComponent::add_layer(int layer) {
    layers.insert(layer);
}

void AreaComponent::remove_layer(int layer) {
    layers.insert(layer);
}

void AreaComponent::toggle_layer(int layer) {
    if (layers.find(layer) != layers.end()) layers.erase(layer);

    else layers.insert(layer);
}

void AreaComponent::set_layers(std::set<int> new_layers) {
    layers.clear();

    for (int layer: new_layers) {
        layers.insert(layer);
    }
}

std::set<int>& AreaComponent::get_layers() {
    return layers;
}

// Area mask manipulation
void AreaComponent::set_mask_bit(int layer, bool enabled) {
    if (enabled)
        mask.insert(layer);
    else
        mask.erase(layer);
}

void AreaComponent::add_mask_bit(int layer) {
    mask.insert(layer);
}

void AreaComponent::remove_mask_bit(int layer) {
    mask.insert(layer);
}

void AreaComponent::toggle_mask_bit(int layer) {
    if (mask.find(layer) != mask.end()) mask.erase(layer);

    else mask.insert(layer);
}

void AreaComponent::set_mask(std::set<int> new_mask) {
    mask.clear();

    for (int layer: new_mask) {
        mask.insert(layer);
    }
}

std::set<int>& AreaComponent::get_mask() {
    return mask;
}

// TODO: functions for retrieving the first area it's overlapping, signal for when areas are entering and exiting

// Checks and resolves areaision among other areas in the same layers
void AreaComponent::check_overlaps() {
    update_shape_position();

    for (auto layer: mask) {
        for (auto area: AreaManager::get_nearby_areas(this, layer)) {

            if (area != this) {
                if (overlaps(this, area)) {
                    
                    // Add area if its not in
                    if (areas_overlapping.find(area) == areas_overlapping.end()) {
                        areas_overlapping.insert(area);
                    }
                } else {
                    
                    // Remove area if it is in
                    if (areas_overlapping.find(area) != areas_overlapping.end()) {
                        areas_overlapping.erase(area);
                    }
                }
            }
        }
    }
}

// Draws the shape for debugging purposes
void AreaComponent::debug_draw() {

    Vector2 camera_pos = Vector2Add(CameraManager::get_camera()->target, CameraManager::get_camera()->offset);
    if (Vector2Distance(camera_pos, position) > res.x * sqrt(2.f) + 1000) return;

    if (is_circle) {
        Circle *circle = (Circle *)shape;

        DrawCircleLines(circle->x, circle->y, circle->radius, {0, 255, 0, 255});
        DrawCircle(circle->x, circle->y, circle->radius, {0, 255, 0, 20});

    } else if (is_rectangle) {
        Rectangle *rect = (Rectangle *)shape;

        DrawRectangleLines(rect->x - rect->width*.5f, rect->y - rect->height*.5f, rect->width, rect->height, {0, 0, 255, 255});
        DrawRectangle(rect->x - rect->width*.5f, rect->y - rect->height*.5f, rect->width, rect->height, {0, 0, 255, 20});
    }
}

// Processes area component
void AreaComponent::process(float delta) {
    update_shape_position();
}

// Updates position of the areas shape
void AreaComponent::update_shape_position() {
    if (is_circle) {
        Circle *circle = (Circle *)shape;
        circle->x = position.x;
        circle->y = position.y;

    } else if (is_rectangle) {
        Rectangle *rect = (Rectangle *)shape;
        rect->x = position.x;
        rect->y = position.y;
    }
}

// <Area manager>
std::vector<AreaManager::AreaLayer> AreaManager::area_layers {};

// Creates the layers
void AreaManager::init() {
    for (int i = 0; i < (int)AreaIndex::COUNT; i++) {

        area_layers.push_back({});
    }
}

// Gets all areas in a chunk
AreaManager::AreaChunk AreaManager::get_chunk(AreaLayer &layer, int x, int y) {
    std::pair<int, int> chunk_pos = std::make_pair(x, y);

    if (layer.find(chunk_pos) != layer.end())
        return layer[chunk_pos];
    else
        return {};
}

// Returns all areas in a layer that are in a 5x5 chunk area around the given component
AreaManager::AreaChunk AreaManager::get_nearby_areas(AreaComponent *comp, int layer) {
    AreaChunk areas {};

    int x = comp->position.x / AREA_CHUNK_SIZE,
        y = comp->position.y / AREA_CHUNK_SIZE;

    auto &layer_ref = area_layers[layer];

    for (int X = -2; X <= 2; X++) {
        for (int Y = -2; Y <= 2; Y++) {
            AreaChunk chunk = get_chunk(layer_ref, x + X, y + Y);
            areas.insert(areas.end(), chunk.begin(), chunk.end());
        }
    }

    return areas;
}

// Clears the layer array and adds in the areas again, O(n^2)
void AreaManager::reload_areas() {
    for (auto& layer: area_layers) {
        layer.clear();
    }

    for (auto component: ComponentManager::query_components(CompType::AREA)) {
        auto area_component = (AreaComponent *)component;

        std::pair<int, int> chunk_pos = std::make_pair(
            area_component->position.x / AREA_CHUNK_SIZE,
            area_component->position.y / AREA_CHUNK_SIZE
        );

        // Puts the area in a chunk in every one of its layers
        for (int layer: area_component->get_layers()) {
            auto &layer_ref = area_layers[layer];

            if (layer_ref.find(chunk_pos) == layer_ref.end())
                layer_ref[chunk_pos] = {};
            else
                layer_ref[chunk_pos].push_back(area_component);
        }
    }
}

// Calls debug draw on all areas
void AreaManager::draw_debug() {
    for (auto component: ComponentManager::query_components(CompType::AREA)) {

        AreaComponent *area_component = (AreaComponent *)component;
        if (DRAW_AREAS) area_component->debug_draw();
    }
}