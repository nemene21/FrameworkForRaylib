#include <iostream>
#include <time.h>
#include <framework.h>

int main() {
    srand(time(NULL));

    const int window_width = 1920*.5, window_height = 1080*.5;
    InitWindow(window_width, window_height, "Raylib Window!");
    // SetTargetFPS(120);

    // Test data
    Easing::InitEasingFuncs();

    Trail trail = Trail({0, 0}, 12, 16, WHITE, Color{255, 255, 255, 0});
    trail.random_offset = 8;

    Sprite sprite = Sprite("test.png",
        Vector2{100, 100},
        Vector2{1, .5},
        35
    );

    ParticleSystem particle_sys = ParticleSystem("test.json", {200, 200});

    sprite.set_shader("test.glsl");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        float delta = GetFrameTime();

        // FPS counter
        std::string fps_string = std::to_string(GetFPS());
        const char* fps_cstring = fps_string.c_str();
        SetWindowTitle(fps_cstring);

        // Processing data managers
        TextureManager::unload_check();
        ParticleDataManager::unload_check();
        ShaderManager::unload_check();
        ShaderManager::update_uniforms();

        // Hot reloading
        if (TryingToHotReload()) {
            TextureManager::reload();
            ShaderManager::reload();
            ParticleDataManager::reload();
        }

        // Object testing
        particle_sys.set_position(GetMousePosition());
        particle_sys.process(delta);
        particle_sys.draw();

        trail.set_position(GetMousePosition());
        trail.process(delta);
        trail.draw();

        sprite.set_position(GetMousePosition());
        sprite.draw();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}