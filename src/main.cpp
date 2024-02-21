#include <iostream>
#include <time.h>
#include <framework.h>

void process(float delta) {
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

    SceneManager::scene_on->process(delta);
    SceneManager::scene_on->process_entities(delta);
}

void draw_background(float delta) {
    SceneManager::scene_on->draw_entities(delta);
}

void draw_foreground(float delta) {
    SceneManager::scene_on->draw_entities(delta);
}

int main() {
    srand(time(NULL));

    // Rendering init
    InitWindow(1920, 1080, "Raylib Window!");

    RenderTexture2D background  = LoadRenderTexture(1920, 1080);
    RenderTexture2D foreground  = LoadRenderTexture(1920, 1080);
    RenderTexture2D composition = LoadRenderTexture(1920, 1080);
    // ToggleFullscreen();
    SetTargetFPS(120);

    Easing::InitEasingFuncs();

    SceneManager::setup_scene(new TestScene());
    SceneManager::set_scene("test_scene");

    ShaderPtr post_processing_ptr = ShaderManager::get("post_processing.glsl"); 
    Texture2D noise_texture = LoadTexture("assets/images/noise.png");

    // Test data
    Sprite bg = Sprite{"concept2.png", Vector2{1920*.5, 1080*.5}};

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Object processing test
        process(delta);

        // Drawing start
        BeginDrawing();

        // Background
        BeginTextureMode(background);

        ClearBackground(Color{0, 0, 0, 255});
        draw_background(delta);

        EndTextureMode();

        // Foreground
        BeginTextureMode(foreground);
        ClearBackground({0, 0, 0, 0});

        // Object drawing test
        EndShaderMode();
        bg.draw();

        draw_foreground(delta);

        DrawCircle(100, 100, 40, WHITE);

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

        float timer = GetTime();
        Shader post_processing = *post_processing_ptr.get();
        SetShaderValue(post_processing, GetShaderLocation(post_processing, "time"), &timer, SHADER_UNIFORM_FLOAT);
        
        BeginShaderMode(post_processing);
        SetShaderValueTexture(post_processing, GetShaderLocation(post_processing, "noise_texture"), noise_texture);
    
        DrawTexturePro(composition.texture,
            {0, 0, 1920, 1080},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            {0, 0},
            0,
            Color{255, 255, 255}
        );
        EndShaderMode();

        EndDrawing();

        // FPS counter
        std::string fps_string = std::to_string(GetFPS());
        const char* fps_cstring = fps_string.c_str();
        SetWindowTitle(fps_cstring);
    }

    // Unload remaining assets
    TextureManager::unload_all();
    ParticleDataManager::unload_all();
    ShaderManager::unload_all();
    SceneManager::unload_all();

    UnloadTexture(noise_texture);

    CloseWindow();

    return 0;
}