#include <iostream>
#include <raylib.h>
#include <raymath.h>

int main() {
    std::cout << "Lorem ipsum" << std::endl;

    const int window_width = 1920, window_height = 1080;
    InitWindow(window_width, window_height, "Raylib Window!");

    ToggleFullscreen();

    float pressed = .0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        pressed = Lerp(pressed, float(IsMouseButtonDown(MOUSE_BUTTON_LEFT)), 0.02);

        unsigned char color_intensity = pressed * 255.;
        ClearBackground(Color{0, color_intensity, color_intensity, 255});

        EndDrawing();
    }

    CloseWindow();

    return 0;
}