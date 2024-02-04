#include <trails.h>

Trail::Trail(
        Vector2 position, float width, int max_points, Color color, Color fade_color
    ):
        position {position},
        width {width},
        max_points {max_points},
        color {color},
        fade_color {fade_color},
        tick {.025},
        timer {0} {
            if (fade_color == WHITE)
                fade_color = color;
        }

void Trail::spawn_point() {
    Vector2 offset {(float)GetRandomValue(0, random_offset), 0};
    offset = Vector2Rotate(offset, RandF() * PI * 2);

    Vector2 new_point = Vector2Add(position, offset);
    points.push(new_point);
}

void Trail::process() {

    timer -= GetFrameTime();
    if (timer < .0) {
        timer = tick;

        spawn_point();
    }

    if ((int)points.size() > max_points) {
        points.pop();
    }
}

void Trail::draw() {
    int point_count = points.size();

    for (int i = 0; i < point_count; i++) {
        Vector2& first_point = points.front();
        points.pop();

        Vector2& other_point = points.front();
        points.push(first_point);

        if (i != point_count-1) {
            float anim = i/(float)point_count;

            Color color_calc = Lerp(fade_color, color, anim);

            DrawLineEx(first_point, other_point, width*anim, color_calc);
            // DrawCircleV(first_point, width*anim * .5, color_calc);
        }
    }
}
