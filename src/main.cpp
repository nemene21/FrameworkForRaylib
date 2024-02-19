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
    SetTargetFPS(120);

    typedef struct {
        Vector2 pos;
        int r, g, b, radius;
    } Light;

    Shader lighting_shader = LoadShader(NULL, "assets/shaders/lighting.glsl");
    std::vector<Light> lights;
    
    Image light_data = GenImageColor(96*2, 1, BLACK);

    lights.push_back(
        Light{0, 0, 255, 255, 255, 20}
    );

    lights.push_back(
        Light{200, 200, 255, 0, 0, 20}
    );

    Easing::InitEasingFuncs();

    // Test data
    Trail trail = Trail({1920*.5, 1080*.5}, 12, 16, WHITE, Color{255, 255, 255, 0});
    trail.random_offset = 8;

    Sprite sprite = Sprite("test.png",
        Vector2{100, 100},
        Vector2{1, .5},
        35
    );
    sprite.set_shader("test.glsl");

    Sprite bg = Sprite("concept2.png", {1920*.5, 1080*.5});

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

        EndShaderMode();

        bg.draw();

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

        BeginShaderMode(lighting_shader);

        for (size_t i = 0; i < lights.size(); i++) {
            size_t ofst = i*8;

            ((unsigned char*)light_data.data)[ofst    ] = (unsigned char)(lights[i].pos.x  / 1920.0 * 255.0);
            ((unsigned char*)light_data.data)[ofst + 1] = (unsigned char)(lights[i].pos.y  / 1080.0 * 255.0);
            ((unsigned char*)light_data.data)[ofst + 2] = (unsigned char)(lights[i].radius / 1920.0 * 255.0);
        
            ((unsigned char*)light_data.data)[ofst + 4] = (unsigned char)(lights[i].r);
            ((unsigned char*)light_data.data)[ofst + 5] = (unsigned char)(lights[i].g);
            ((unsigned char*)light_data.data)[ofst + 6] = (unsigned char)(lights[i].b);
        }

        Texture light_data_texture = LoadTextureFromImage(light_data);
        SetShaderValue(lighting_shader, GetShaderLocation(lighting_shader, "lights"), &light_data_texture, SHADER_UNIFORM_SAMPLER2D);
        UnloadTexture(light_data_texture);

        int num_lights = lights.size();
        SetShaderValue(lighting_shader, GetShaderLocation(lighting_shader, "num_lights"), &num_lights, SHADER_UNIFORM_INT);

        DrawTexturePro(foreground.texture,
            {0, 0, 1920, 1080},
            {0, 0, 1920, 1080},
            {0, 0},
            0,
            WHITE
        );
        EndShaderMode();

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
    TextureManager::unload_all();
    ParticleDataManager::unload_all();
    ShaderManager::unload_all();

    CloseWindow();

    return 0;
}