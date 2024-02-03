#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <test.h>

int main() {
    std::cout << "Lorem ipsum" << std::endl;

    const int window_width = 1920, window_height = 1080;
    InitWindow(window_width, window_height, "Raylib Window!");

    ToggleFullscreen();

    while (!WindowShouldClose()) {
        BeginDrawing();

        unsigned char color_intensity = abs(sin(GetTime())) * 255.;
        ClearBackground(Color{0, color_intensity, color_intensity, 255});

        EndDrawing();
    }

    VERYCOOLFUNCTION(50);
    std::cout << "me when" << std::endl;

    CloseWindow();

    return 0;
}