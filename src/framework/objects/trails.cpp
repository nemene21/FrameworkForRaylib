#include <trails.hpp>

// Trail constructor
Trail::Trail(
    Vector2 position, float width, int max_points, Color color, Color fade_color
):
    position {position},
    width {width},
    max_points {max_points},
    color {color},
    fade_color {fade_color},
    force {0, 0},
    tick {.025},
    timer {0},
    random_offset {random_offset}
    {}

// For adders
void Trail::add_force(Vector2 new_force) {
    force = Vector2Add(force, new_force);
}

void Trail::remove_force(Vector2 force_removing) {
    force = Vector2Subtract(force, force_removing);
}

// Tick setter
void Trail::set_tick(float new_tick) {
    tick = new_tick;
}

// Position setter and getter
void Trail::set_position(Vector2 new_pos) {
    position = new_pos;
}

Vector2 Trail::get_position() {
    return position;
}

// Spawns new point at the end of the trail
void Trail::spawn_point() {
    TrailPoint new_point;

    Vector2 offset {(float)GetRandomValue(0, random_offset), 0};
    offset = Vector2Rotate(offset, RandF() * PI * 2);

    new_point.position = Vector2Add(position, offset);
    new_point.velocity = Vector2{0.f, 0.f};

    points.push(new_point);
}

// Processes a point
void Trail::process_point(TrailPoint& point, float delta) {

    point.velocity = Vector2Add(
        point.velocity,
        Vector2Multiply(force, Vector2{delta, delta})
    );

    point.position = Vector2Add(
        point.position,
        Vector2Multiply(point.velocity, Vector2{delta, delta})
    );
}

// Processes the trail object
void Trail::process(float delta) {
    // Process all points
    for (int i = 0; i < (int)points.size(); i++) {

        auto& point = points.front();
        process_point(point, delta);
        
        points.pop();
        points.push(point);
    }

    // New point timer
    timer -= delta;
    if (timer < .0) {
        timer = tick;

        spawn_point();
    }

    // Remove point if the point amount is too large
    if ((int)points.size() > max_points) {
        points.pop();
    }
}

void Trail::draw() {
    int point_count = points.size();

    for (int i = 0; i < point_count; i++) {

        // Get points
        TrailPoint& first_point = points.front();
        points.pop();

        TrailPoint& other_point = points.front();
        points.push(first_point);

        // Animation and drawing calculations
        float anim = i/(float)point_count;
        Color color_calc = Lerp(fade_color, color, anim); // Get animated color

        // Draw line and circle if its not the last point
        if (i != point_count-1) {

            DrawLineEx(
                first_point.position,
                other_point.position,
                width*anim, color_calc
            );
        }
        // DrawCircleV(first_point.position, width*anim * .5, color_calc);
    }
}
