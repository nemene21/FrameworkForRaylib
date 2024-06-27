#ifndef TRAILS_HEADER
#define TRAILS_HEADER

#include <raylib.h>
#include <raymath.h>

#include <misc.hpp>

#include <iostream>
#include <vector>
#include <queue>
#include <drawables.hpp>

/// @brief Draws a shrinking trail with a color gradient
class Trail: public Drawable {
protected:
    typedef struct {
        Vector2 position, velocity;
    } TrailPoint;

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

    /// @brief Sets the amount of time between updating trail point count
    /// @param tick 
    void set_tick(float tick);
    /// @brief Adds force that affects trail's points
    /// @param new_force Force to add
    void add_force(Vector2 new_force);
    /// @brief Subtracts force that affects trail's points
    /// @param force_removing Force to subtract
    void remove_force(Vector2 force_removing);

    /// @brief Spawns the next point in the trail
    void spawn_point();
    /// @brief Processes a single point in the trail
    /// @param point Point pointer
    /// @param delta Time since last update
    void process_point(TrailPoint& point, float delta);

    void process(float delta);
    void draw();
};


#endif