#include <iostream>
#include <time.h>
#include <framework.h>

void process() {
    float delta = GetFrameTime();

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
}

void draw() {

}

int main() {
    srand(time(NULL));

    // Rendering init
    InitWindow(1920, 1080, "Raylib Window!");

    RenderTexture2D background  = LoadRenderTexture(1920, 1080);
    RenderTexture2D foreground  = LoadRenderTexture(1920, 1080);
    RenderTexture2D composition = LoadRenderTexture(1920, 1080);
    // ToggleFullscreen();
    // SetTargetFPS(120);

    Easing::InitEasingFuncs();

    // Test data
    Trail trail = Trail({0, 0}, 12, 16, WHITE, Color{255, 255, 255, 0});
    trail.random_offset = 8;

    Sprite sprite = Sprite("test.png",
        Vector2{100, 100},
        Vector2{1, .5},
        35
    );
    sprite.set_shader("test.glsl");

    ParticleSystem particle_sys = ParticleSystem("test.json", {200, 200});

    while (!WindowShouldClose()) {

        float delta = GetFrameTime();

        // Object processing test
        process();

        particle_sys.set_position(GetMousePosition());
        particle_sys.process(delta);

        trail.set_position(GetMousePosition());
        trail.process(delta);

        sprite.set_position(GetMousePosition());

        // Drawing start
        BeginDrawing();
        BeginTextureMode(background);

        ClearBackground(BLACK);

        EndTextureMode();

        BeginTextureMode(foreground);
        ClearBackground({0, 0, 0, 0});

        // Object drawing test
        particle_sys.draw();

        trail.draw();

        sprite.draw();
        draw();

        EndTextureMode();
        BeginTextureMode(composition);

        DrawTexturePro(background.texture,
            {0, 0, 1920, 1080},
            {0, 0, 1920, 1080},
            {0, 0},
            0,
            WHITE
        );

        DrawTexturePro(foreground.texture,
            {0, 0, 1920, 1080},
            {0, 0, 1920, 1080},
            {0, 0},
            0,
            WHITE
        );

        EndTextureMode();

        DrawTexturePro(composition.texture,
            {0, 0, 1920, 1080},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            {0, 0},
            0,
            WHITE
        );

        EndDrawing();

        // FPS counter
        std::string fps_string = std::to_string(GetFPS());
        const char* fps_cstring = fps_string.c_str();
        SetWindowTitle(fps_cstring);
    }

    // Unload remaining assets
    TextureManager::unload_check();
    ParticleDataManager::unload_check();
    ShaderManager::unload_check();
    ShaderManager::update_uniforms();

    CloseWindow();

    return 0;
}