#include <collider_component.hpp>

bool collides(ColliderComponent *coll1, ColliderComponent *coll2) {
    if (coll1->is_rectangle && coll2->is_rectangle) {
        return CheckCollisionRecs( *(Rectangle *)coll1->shape, *(Rectangle *)coll2->shape );
    }
    if (coll1->is_rectangle && coll2->is_circle) {
        Circle *circle = (Circle *)coll2->shape;
        return CheckCollisionCircleRec({circle->x, circle->y}, circle->radius, *(Rectangle *)coll1->shape);
    }
    if (coll2->is_rectangle && coll1->is_circle) {
        Circle *circle = (Circle *)coll1->shape;
        return CheckCollisionCircleRec({circle->x, circle->y}, circle->radius, *(Rectangle *)coll2->shape);
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

        collision_point.x = Clamp(collision_point.x, coll2->position.x - shape2.width, coll2->position.x + shape2.width);
        collision_point.y = Clamp(collision_point.y, coll2->position.y - shape2.height, coll2->position.y + shape2.height);
    }

    if (coll1->is_rectangle) {

    } else if (coll1->is_circle) {

    }
}

ColliderComponent::ColliderComponent(Entity *entity, float width, float height):
    Component(CompType::COLLIDER, entity),
    shape {nullptr},
    is_rectangle {true},
    is_circle {false}
{
    std::cout << "Gonna init shape!!!" << std::endl;
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
        for (auto collider: ColliderManager::get_nearby_colliders(this)) {

            if (collides(this, collider)) {
                resolve_collision(direction, this, collider);
            }
        }
    }
}

void ColliderComponent::draw(float delta) {
    if (is_circle) {
        Circle *circle = (Circle *)shape;

        DrawCircleLines(circle->x, circle->y, circle->radius, {0, 0, 255, 255});
    } else if (is_rectangle) {
        Rectangle *rect = (Rectangle *)shape;

        DrawRectangleLines(rect->x, rect->y, rect->width, rect->height, {0, 0, 255, 255});
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

ColliderManager::ColliderChunk ColliderManager::get_nearby_colliders(ColliderComponent *comp) {

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

        for (int layer: collider_component->get_layers()) {
            auto &layer_ref = collider_layers[layer];

            if (layer_ref.find(chunk_pos) == layer_ref.end())
                layer_ref[chunk_pos] = {};
            else
                layer_ref[chunk_pos].push_back(collider_component);
        }
    }
}