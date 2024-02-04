
#ifndef TRAILS_H
#define TRAILS_H

#include <raylib.h>
#include <raymath.h>
#include <queue>
#include <iostream>
#include <misc.h>

class Trail {
public:
    Vector2 position;
    std::queue<Vector2> points;

    float width;
    int max_points;
    Color color, fade_color;

    Trail(Vector2 position=Vector2{0, 0},
        float width=0,
        int max_points=16,
        Color color=WHITE,
        Color fade_color=WHITE
    );

    void process();

    void draw();
};


#endif