
#ifndef TRAILS_H
#define TRAILS_H

#include <raylib.h>
#include <raymath.h>
#include <queue>
#include <iostream>
#include <misc.h>
#include <vector>

class Trail {
protected:
    typedef struct {
        Vector2 position, velocity;
    } TrailPoint;

    Vector2 position;
    float width;
    int max_points;

    Color color, fade_color;

    Vector2 force;
    std::queue<TrailPoint> points;

    float tick, timer;

public:
    float random_offset;

    Trail(Vector2 position=Vector2{0, 0},
        float width=0,
        int max_points=16,
        Color color=WHITE,
        Color fade_color=WHITE
    );
    void set_tick(float tick);

    void add_force(Vector2 new_force);
    void remove_force(Vector2 force_removing);

    void spawn_point();
    void process_point(TrailPoint& point, float delta);

    void process(Vector2 new_position);
    void draw();
};


#endif