#include <framework.hpp>

RenderTexture2D ui_layer, game_layer, composition_layer;

ShaderPtr post_processing_ptr; 
TexturePtr noise_texture,
           paper_texture;

void Framework::init(std::string title, Vector2 resolution, int window_scale) {
    srand(time(NULL));
    res = resolution;

    // Init
    InitWindow(res.x * window_scale, res.y * window_scale, (title).c_str());
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);

    ui_layer   = LoadRenderTexture(res.x, res.y);
    game_layer = LoadRenderTexture(res.x, res.y);
    composition_layer = LoadRenderTexture(res.x, res.y);
    SetTargetFPS(60);

    InitAudioDevice();

    // Default camera
    Camera2D* blank_camera = new Camera2D();
    blank_camera->target = {0, 0};
    blank_camera->offset = {0, 0};
    blank_camera->rotation = 0;
    blank_camera->zoom = 1;

    CameraManager::bind_camera(blank_camera);

    // Init modules
    Easing::InitEasingFuncs();
    CameraManager::init();
    ColliderManager::init();
    AreaManager::init();

    // Loading post processing shader and it's textures
    post_processing_ptr = ShaderManager::get("post_processing.glsl"); 
    noise_texture = TextureManager::get("post_processing/noise.png");
    paper_texture = TextureManager::get("post_processing/paper.png");
}

void Framework::process_modules(float delta) {
        // Processing data managers
    TextureManager::unload_check();
    ParticleDataManager::unload_check();
    
    AudioManager::unload_check();
    AudioManager::process_track_fade(delta);

    ShaderManager::unload_check();
    ShaderManager::update_uniforms();

    ColliderManager::reload_colliders();
    AreaManager::reload_areas();

    // Hot reloading
    if (TryingToHotReload()) {
        TextureManager::reload();
        ShaderManager::reload();
        AudioManager::reload();
        ParticleDataManager::reload();
    }
}

void Framework::process_scene(float delta) {
    SceneManager::scene_on->process(delta);
    SceneManager::scene_on->process_entities(delta);
}

void Framework::draw_game_layer(float delta) {
    // Game layer
    BeginTextureMode(game_layer);
    BeginMode2D(*global_camera);
    ClearBackground({0, 0, 0, 255});

    SceneManager::scene_on->draw_entities(delta);
    DrawableManager::draw();
    
    ColliderManager::draw_debug();
    AreaManager::draw_debug();

    EndMode2D();        
    EndTextureMode();
}

void Framework::run() {
    while (!WindowShouldClose()) {
        // Delta time calc
        float delta = fminf(GetFrameTime(), 1/20.f);

        // Resizing window
        if (IsWindowResized()) {
            SetWindowSize(GetScreenWidth(), GetScreenHeight());
        }

        // Processing
        process_modules(delta);
        process_scene(delta);

        // Drawing start 
        BeginDrawing();
        Framework::draw_game_layer(delta);

        // Compose UI and game layer
        BeginTextureMode(composition_layer);
        DrawTexturePro(game_layer.texture,
            {0, 0, res.x, res.y},
            {0, 0, res.x, res.y},
            {0, 0},
            0,
            WHITE
        );
        EndTextureMode();

        // Post processing uniforms
        Shader post_processing = *post_processing_ptr.get();
        BeginShaderMode(post_processing);

        float timer = GetTime();
        float aspect_ratio = res.x / res.y;

        SetShaderValue(post_processing, GetShaderLocation(post_processing, "time"), &timer, SHADER_UNIFORM_FLOAT);
        SetShaderValue(post_processing, GetShaderLocation(post_processing, "aspect_ratio"), &aspect_ratio, SHADER_UNIFORM_FLOAT);
        
        // Post processing texture binding
        SetShaderValueTexture(post_processing, GetShaderLocation(post_processing, "noise_texture"), *noise_texture.get());
        SetShaderValueTexture(post_processing, GetShaderLocation(post_processing, "paper_texture"), *paper_texture.get());

        Vector2 camera_offset = Vector2Divide(global_camera->target, res);
        SetShaderValue(
            post_processing,
            GetShaderLocation(post_processing, "camera_offset"),
            &camera_offset,
            SHADER_ATTRIB_VEC2
        );

        // Drawing composition layer on display
        float draw_width  = GetScreenWidth();
        float draw_height = GetScreenWidth() * (res.y / res.x);

        if (draw_height > GetScreenHeight()) {
            draw_height = GetScreenHeight();
            draw_width  = GetScreenHeight() * (res.x / res.y);
        }

        ClearBackground(BLACK);
        DrawTexturePro(composition_layer.texture,
            {0, 0, res.x, res.y},
            {(GetScreenWidth() - draw_width) * .5f, (GetScreenHeight() - draw_height) * .5f, draw_width, draw_height},
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
    deinit();
}

void Framework::deinit() {
    // Unload remaining assets
    TextureManager::unload_all();
    ParticleDataManager::unload_all();
    ShaderManager::unload_all();
    SceneManager::unload_all();
    AudioManager::unload_all();

    CloseWindow();
}