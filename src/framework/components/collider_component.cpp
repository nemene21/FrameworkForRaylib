#include <collider_component.hpp>

ColliderComponent::ColliderComponent(): Component(CompType::COLLIDER, nullptr) {}

bool collides(ColliderComponent *coll1, ColliderComponent *coll2) {
    if (coll1->is_rectangle && coll2->is_rectangle) {
        Rectangle shape1 = *(Rectangle *)coll1->shape;
        Rectangle shape2 = *(Rectangle *)coll2->shape;
        return CheckCollisionRecs(
            {shape1.x - shape1.width*.5f, shape1.y - shape1.height*.5f, shape1.width, shape1.height},
            {shape2.x - shape2.width*.5f, shape2.y - shape2.height*.5f, shape2.width, shape2.height}
        );
    }
    if (coll1->is_rectangle && coll2->is_circle) {
        Circle *circle = (Circle *)coll2->shape;
        Rectangle rect = *(Rectangle *)coll1->shape;

        return CheckCollisionCircleRec({circle->x, circle->y}, circle->radius, {
            rect.x - rect.width*.5f, rect.y - rect.height*.5f, rect.width, rect.height
        });
    }
    if (coll2->is_rectangle && coll1->is_circle) {
        Circle *circle = (Circle *)coll1->shape;
        Rectangle rect = *(Rectangle *)coll2->shape;
        
        return CheckCollisionCircleRec({circle->x, circle->y}, circle->radius, {
            rect.x - rect.width*.5f, rect.y - rect.height*.5f, rect.width, rect.height
        });
    }
    Circle *circle1 = (Circle *)coll1->shape;
    Circle *circle2 = (Circle *)coll2->shape;

    return CheckCollisionCircles({circle1->x, circle1->y}, circle1->radius, {circle2->x, circle2->y}, circle2->radius);
}

// Collision resolution
void resolve_collision(Vector2 direction, ColliderComponent *coll1, ColliderComponent *coll2) {
    Vector2 collision_point {0, 0};

    if (coll2->is_circle) {
        // ...
    }
    else if (coll2->is_rectangle) {
        // Gets the collision point by trapping the first collider's position within the second rectangle collider
        Rectangle shape2 = *(Rectangle *)coll2->shape;
        collision_point = coll1->position;

        collision_point.x = Clamp(collision_point.x, coll2->position.x - shape2.width*.5f, coll2->position.x + shape2.width*.5f);
        collision_point.y = Clamp(collision_point.y, coll2->position.y - shape2.height*.5f, coll2->position.y + shape2.height*.5f);
    }

    if (coll1->is_rectangle) {
        // Solves first colliders position if it's a rectangle
        Rectangle shape = *(Rectangle *)coll1->shape;

        if (direction.x != 0) {
            coll1->position.x = collision_point.x + (shape.width) * ((int)(direction.x < 0) * 2.f - 1.f) * .500001f - (shape.width) * .0000005f;
        }
        if (direction.y != 0) {
            coll1->position.y = collision_point.y + (shape.height) * ((int)(direction.y < 0) * 2.f - 1.f) * .500001f - (shape.height) * .0000005f;
        }
    } else if (coll1->is_circle) {

    }
}
// Collider rectangle init
ColliderComponent::ColliderComponent(Entity *entity, float width, float height):
    Component(CompType::COLLIDER, entity),
    shape {nullptr},
    is_rectangle {true},
    is_circle {false},
    collision_direction {0, 0},
    position {0, 0}
{
    shape = new Rectangle{0, 0, width, height};
}

// Collider circle init (crashes due to unimplemented circles)
ColliderComponent::ColliderComponent(Entity *entity, float radius):
    Component(CompType::COLLIDER, entity),
    shape {nullptr},
    is_rectangle {false},
    is_circle {true}
{
    std::cout << "Didnt add circles yet :(" << std::endl;
    exit(0);

    shape = new Circle{0, 0, radius};
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

    Vector2 camera_pos = Vector2Add(CameraManager::get_camera()->target, CameraManager::get_camera()->offset);
    if (Vector2Distance(camera_pos, position) > res.x * sqrt(2.f) + 1000) return;

    if (is_circle) {
        Circle *circle = (Circle *)shape;

        DrawCircleLines(circle->x, circle->y, circle->radius, {0, 0, 255, 255});
        DrawCircle(circle->x, circle->y, circle->radius, {0, 0, 255, 20});

    } else if (is_rectangle) {
        Rectangle *rect = (Rectangle *)shape;

        DrawRectangleLines(rect->x - rect->width*.5f, rect->y - rect->height*.5f, rect->width, rect->height, {0, 0, 255, 255});
        DrawRectangle(rect->x - rect->width*.5f, rect->y - rect->height*.5f, rect->width, rect->height, {0, 0, 255, 20});
    }
}

// Processes collider component
void ColliderComponent::process(float delta) {
    update_shape_position();
}

// Updates position of the colliders shape
void ColliderComponent::update_shape_position() {
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
        if (DRAW_COLLIDERS) collider_component->debug_draw();
    }
}