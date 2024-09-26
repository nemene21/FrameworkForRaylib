#include <collider_component.hpp>

bool DRAW_COLLIDERS = false;

ColliderComponent::ColliderComponent(Vector2 pos):
    Component(CompType::COLLIDER, nullptr), draw_debug {false}
     {
    position = pos;
    update_shape_position();
}

bool collides(ColliderComponent *coll1, ColliderComponent *coll2) {
    return CheckCollisionRecs({
        coll1->shape.x - .5f*coll1->shape.width,
        coll1->shape.y - .5f*coll1->shape.height,
        coll1->shape.width,  coll1->shape.height
    }, {
        coll2->shape.x - .5f*coll2->shape.width,
        coll2->shape.y - .5f*coll2->shape.height,
        coll2->shape.width,  coll2->shape.height
    });
}

// Collision resolution
void resolve_collision(Vector2 direction, ColliderComponent *coll1, ColliderComponent *coll2) {
    if (direction.x != 0) {
        float distance = (coll1->shape.width + coll2->shape.width) * .5f;
        coll1->position.x = coll2->position.x - distance * direction.x;
        
    } else if (direction.y != 0) {
        float distance = (coll1->shape.height + coll2->shape.height) * .5f;
        coll1->position.y = coll2->position.y - distance * direction.y;
    }
}
// Collider rectangle init
ColliderComponent::ColliderComponent(Entity *entity, Vector2 pos, float width, float height):
    Component(CompType::COLLIDER, entity),
    shape {Rectangle{pos.x, pos.y, width, height}},
    draw_debug {false},
    collision_direction {0, 0},
    position {pos}
{
    update_shape_position();
}

void ColliderComponent::draw_gui_info() {
    if (ImGui::CollapsingHeader(("Collider##" + std::to_string(id)).c_str())) {
        ImGui::Indent(25.f);
        ImGui::Checkbox(("Debug draw##" + std::to_string(id)).c_str(), &draw_debug);
        
        ImGui::Text("Touching: ");
        ImGui::SameLine();
        ImGui::TextColored(on_ceil()  ? ImVec4(0, 1, 0, 1) : ImVec4(1, 1, 1, .5), "Up ");
        ImGui::SameLine();
        ImGui::TextColored(on_floor() ? ImVec4(0, 1, 0, 1) : ImVec4(1, 1, 1, .5), "Down ");
        ImGui::SameLine();
        ImGui::TextColored(on_left_wall()  ? ImVec4(0, 1, 0, 1) : ImVec4(1, 1, 1, .5), "Left ");
        ImGui::SameLine();
        ImGui::TextColored(on_right_wall() ? ImVec4(0, 1, 0, 1) : ImVec4(1, 1, 1, .5), "Right");

        ImGui::Unindent(25.f);
    }
}


// Collider collision direction checks
bool ColliderComponent::on_floor() {
    return collision_direction.y > 0;
}

bool ColliderComponent::on_ceil() {
    return collision_direction.y < 0;
}

bool ColliderComponent::on_left_wall() {
    return collision_direction.x < 0;
}

bool ColliderComponent::on_right_wall() {
    return collision_direction.x > 0;
}

bool ColliderComponent::on_wall() {
    return collision_direction.x != 0;
}

// Collider layer manipulation
void ColliderComponent::set_layer(int layer, bool enabled) {
    if (enabled)
        layers.insert(layer);
    else
        layers.erase(layer);
}

void ColliderComponent::add_layer(int layer) {
    layers.insert(layer);
}

void ColliderComponent::remove_layer(int layer) {
    layers.insert(layer);
}

void ColliderComponent::toggle_layer(int layer) {
    if (layers.find(layer) != layers.end()) layers.erase(layer);

    else layers.insert(layer);
}

void ColliderComponent::set_layers(std::set<int> new_layers) {
    layers.clear();

    for (int layer: new_layers) {
        layers.insert(layer);
    }
}

std::set<int>& ColliderComponent::get_layers() {
    return layers;
}

// Collider mask manipulation
void ColliderComponent::set_mask_bit(int layer, bool enabled) {
    if (enabled)
        mask.insert(layer);
    else
        mask.erase(layer);
}

void ColliderComponent::add_mask_bit(int layer) {
    mask.insert(layer);
}

void ColliderComponent::remove_mask_bit(int layer) {
    mask.insert(layer);
}

void ColliderComponent::toggle_mask_bit(int layer) {
    if (mask.find(layer) != mask.end()) mask.erase(layer);

    else mask.insert(layer);
}

void ColliderComponent::set_mask(std::set<int> new_mask) {
    mask.clear();

    for (int layer: new_mask) {
        mask.insert(layer);
    }
}

std::set<int>& ColliderComponent::get_mask() {
    return mask;
}

// Checks and resolves collision among other colliders in the same layers
void ColliderComponent::collide(Vector2 direction) {
    if (direction.x != 0) collision_direction.x = 0;
    if (direction.y != 0) collision_direction.y = 0;

    for (auto layer: mask) {
        for (auto collider: ColliderManager::get_nearby_colliders(this, layer)) {

            if (collider != this) {
                update_shape_position();

                if (collides(this, collider)) {

                    if (direction.x != 0) collision_direction.x = direction.x;
                    if (direction.y != 0) collision_direction.y = direction.y;

                    resolve_collision(direction, this, collider);
                }
                update_shape_position();
            }
        }
    }
}

// Draws the shape for debugging purposes
void ColliderComponent::debug_draw() {
    Vector2 camera_pos = Vector2Subtract(CameraManager::get_camera()->target, CameraManager::get_camera()->offset);
    if (Vector2Distance(camera_pos, {shape.x, shape.y}) > shape.width+shape.height + res_diagonal)
        return;

    DrawRectangleLines(shape.x - shape.width*.5f, shape.y - shape.height*.5f, shape.width, shape.height, {0, 0, 255, 255});
    DrawRectangle(shape.x - shape.width*.5f, shape.y - shape.height*.5f, shape.width, shape.height, {0, 0, 255, 20});
}

// Processes collider component
void ColliderComponent::process(float delta) {
    update_shape_position();
}

// Updates position of the colliders shape
void ColliderComponent::update_shape_position() {
    shape.x = position.x;
    shape.y = position.y;
}

// <Collider manager>
std::vector<ColliderManager::ColliderLayer> ColliderManager::collider_layers {};

// Creates the layers
void ColliderManager::init() {
    for (int i = 0; i < (int)ColliderIndex::COUNT; i++) {

        collider_layers.push_back({});
    }
}

// Gets all colliders in a chunk
ColliderManager::ColliderChunk ColliderManager::get_chunk(ColliderLayer &layer, int x, int y) {
    std::pair<int, int> chunk_pos = std::make_pair(x, y);

    if (layer.find(chunk_pos) != layer.end())
        return layer[chunk_pos];
    else
        return {};
}

// Returns all colliders in a layer that are in a 5x5 chunk area around the given component
ColliderManager::ColliderChunk ColliderManager::get_nearby_colliders(ColliderComponent *comp, int layer) {
    ColliderChunk colliders {};

    int x = comp->position.x / COLLIDER_CHUNK_SIZE,
        y = comp->position.y / COLLIDER_CHUNK_SIZE;

    auto &layer_ref = collider_layers[layer];

    for (int X = -2; X <= 2; X++) {
        for (int Y = -2; Y <= 2; Y++) {
            ColliderChunk chunk = get_chunk(layer_ref, x + X, y + Y);
            colliders.insert(colliders.end(), chunk.begin(), chunk.end());
        }
    }

    return colliders;
}

// Clears the layer array and adds in the colliders again, O(n^2)
void ColliderManager::reload_colliders() {
    for (auto& layer: collider_layers) {
        layer.clear();
    }

    for (auto component: ComponentManager::query_components(CompType::COLLIDER)) {
        auto collider_component = (ColliderComponent *)component;

        std::pair<int, int> chunk_pos = std::make_pair(
            collider_component->position.x / COLLIDER_CHUNK_SIZE,
            collider_component->position.y / COLLIDER_CHUNK_SIZE
        );

        // Puts the collider in a chunk in every one of its layers
        for (int layer: collider_component->get_layers()) {
            auto &layer_ref = collider_layers[layer];

            if (layer_ref.find(chunk_pos) == layer_ref.end())
                layer_ref[chunk_pos] = {collider_component};
            else
                layer_ref[chunk_pos].push_back(collider_component);
        }
    }
}

// Calls debug draw on all colliders
void ColliderManager::draw_debug() {
    for (auto component: ComponentManager::query_components(CompType::COLLIDER)) {

        ColliderComponent *collider_component = (ColliderComponent *)component;
        if (DRAW_COLLIDERS || collider_component->draw_debug) collider_component->debug_draw();
    }
}