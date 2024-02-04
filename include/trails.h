
#ifndef TRAILS_H
#define TRAILS_H

#include <raylib.h>
#include <raymath.h>
#include <queue>
#include <iostream>

class Trail {
public:
    Vector2 position;
    std::queue<Vector2> points;

    float width;
    int max_points;
    Color color;

    Trail(Vector2 position, float width, int max_points, Color color);

    void process();

    void draw();
};


#endif