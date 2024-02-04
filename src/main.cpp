#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <trails.h>

int main() {
    const int window_width = 1920*.5, window_height = 1080*.5;
    InitWindow(window_width, window_height, "Raylib Window!");

    Trail trail = Trail({0, 0}, 12, 128, WHITE);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        trail.position = GetMousePosition();
        trail.process();

        trail.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}