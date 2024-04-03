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

void resolve_collision(Vector2 direction, ColliderComponent *coll1, ColliderComponent *coll2) {
    Vector2 collision_point {0, 0};

    if (coll2->is_circle) {
        Circle shape2 = *(Circle *)coll2->shape;
        collision_point = coll2->position;

        float angle = atan2f(collision_point.x - coll1->position.x, collision_point.y - coll1->position.y);
        collision_point = Vector2Add(collision_point, Vector2Rotate({shape2.radius, 0}, angle));

    }
    else if (coll2->is_rectangle) {
        Rectangle shape2 = *(Rectangle *)coll2->shape;
        collision_point = coll1->position;

        collision_point.x = Clamp(collision_point.x, coll2->position.x - shape2.width*.5f, coll2->position.x + shape2.width*.5f);
        collision_point.y = Clamp(collision_point.y, coll2->position.y - shape2.height*.5f, coll2->position.y + shape2.height*.5f);
    }

    if (coll1->is_rectangle) {
        Rectangle shape = *(Rectangle *)coll1->shape;

        if (direction.y == 0) {
            coll1->position.x = collision_point.x + shape.width * ((int)(direction.x < 0) * 2.f - 1.f) * .55f;
        } else
            coll1->position.y = collision_point.y + shape.height * ((int)(direction.y < 0) * 2.f - 1.f) * .55f;

    } else if (coll1->is_circle) {

    }
}

ColliderComponent::ColliderComponent(Entity *entity, float width, float height):
    Component(CompType::COLLIDER, entity),
    shape {nullptr},
    is_rectangle {true},
    is_circle {false}
{
    shape = new Rectangle{0, 0, width, height};
}

ColliderComponent::ColliderComponent(Entity *entity, float radius):
    Component(CompType::COLLIDER, entity),
    shape {nullptr},
    is_rectangle {false},
    is_circle {true}
{
    shape = new Circle{0, 0, radius};
}

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

void ColliderComponent::collide(Vector2 direction) {
    for (auto layer: layers) {
        for (auto collider: ColliderManager::get_nearby_colliders(this, layer)) {

            if (collider != this) {
                process(1);

                if (collides(this, collider)) {
                    resolve_collision(direction, this, collider);
                }
                process(1);
            }
        }
    }
}

void ColliderComponent::debug_draw() {
    if (is_circle) {
        Circle *circle = (Circle *)shape;

        DrawCircleLines(circle->x, circle->y, circle->radius, {0, 0, 255, 255});
    } else if (is_rectangle) {
        Rectangle *rect = (Rectangle *)shape;

        DrawRectangleLines(rect->x - rect->width*.5f, rect->y - rect->height*.5f, rect->width, rect->height, {0, 0, 255, 255});
    }
}

void ColliderComponent::process(float delta) {
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

// Collider manager
std::vector<ColliderManager::ColliderLayer> ColliderManager::collider_layers {};

void ColliderManager::init() {
    for (int i = 0; i < (int)ColliderIndex::COUNT; i++) {

        collider_layers.push_back({});
    }
}

ColliderManager::ColliderChunk ColliderManager::get_chunk(ColliderLayer &layer, int x, int y) {
    std::pair<int, int> chunk_pos = std::make_pair(x, y);

    if (layer.find(chunk_pos) != layer.end())
        return layer[chunk_pos];
    else
        return {};
}


ColliderManager::ColliderChunk ColliderManager::get_nearby_colliders(ColliderComponent *comp, int layer) {
    ColliderChunk colliders {};

    int x = comp->position.x / COLLIDER_CHUNK_SIZE,
        y = comp->position.y / COLLIDER_CHUNK_SIZE;

    auto &layer_ref = collider_layers[layer];
    
    ColliderChunk chunk1 = get_chunk(layer_ref, x, y);
    ColliderChunk chunk2 = get_chunk(layer_ref, x + 1, y);
    ColliderChunk chunk3 = get_chunk(layer_ref, x - 1, y);
    ColliderChunk chunk4 = get_chunk(layer_ref, x, y + 1);
    ColliderChunk chunk5 = get_chunk(layer_ref, x, y - 1);
    ColliderChunk chunk6 = get_chunk(layer_ref, x + 1, y + 1);
    ColliderChunk chunk7 = get_chunk(layer_ref, x - 1, y + 1);
    ColliderChunk chunk8 = get_chunk(layer_ref, x + 1, y - 1);
    ColliderChunk chunk9 = get_chunk(layer_ref, x - 1, y - 1);

    colliders.insert(colliders.end(), chunk1.begin(), chunk1.end());
    colliders.insert(colliders.end(), chunk2.begin(), chunk2.end());
    colliders.insert(colliders.end(), chunk3.begin(), chunk3.end());
    colliders.insert(colliders.end(), chunk4.begin(), chunk4.end());
    colliders.insert(colliders.end(), chunk5.begin(), chunk5.end());
    colliders.insert(colliders.end(), chunk6.begin(), chunk6.end());
    colliders.insert(colliders.end(), chunk7.begin(), chunk7.end());
    colliders.insert(colliders.end(), chunk8.begin(), chunk8.end());
    colliders.insert(colliders.end(), chunk9.begin(), chunk9.end());

    return colliders;
}

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

        // std::cout << chunk_pos.first << ", " << chunk_pos.second << std::endl;

        for (int layer: collider_component->get_layers()) {
            auto &layer_ref = collider_layers[layer];

            if (layer_ref.find(chunk_pos) == layer_ref.end())
                layer_ref[chunk_pos] = {};
            else
                layer_ref[chunk_pos].push_back(collider_component);
        }
    }
}

void ColliderManager::draw_debug() {

    for (auto component: ComponentManager::query_components(CompType::COLLIDER)) {
        ColliderComponent *collider_component = (ColliderComponent *)component;
        collider_component->debug_draw();
    }
}