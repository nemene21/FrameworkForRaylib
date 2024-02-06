#include <iostream>
#include <string>
#include <framework.h>

int main() {
    const int window_width = 1920*.5, window_height = 1080*.5;
    InitWindow(window_width, window_height, "Raylib Window!");

    Trail trail = Trail({0, 0}, 12, 16, WHITE, BLACK);
    trail.random_offset = 8;
    trail.add_force(Vector2{-300., 0.});

    Sprite sprite = Sprite("test.png",
        Vector2{100, 100},
        Vector2{1, .5},
        35
    );

    sprite.set_shader("test.glsl");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        std::string fps_string = std::to_string(GetFPS());

        const char* fps_cstring = fps_string.c_str();
        SetWindowTitle(fps_cstring);

        trail.process(GetMousePosition());

        trail.draw();

        sprite.set_position(GetMousePosition());
        sprite.draw();

        TextureManager::unload_check();
        ShaderManager::unload_check();
        ShaderManager::update_uniforms();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}