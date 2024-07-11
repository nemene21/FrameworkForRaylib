#include <framework.hpp>

RenderTexture2D ui_layer, game_layer, composition_layer;

ShaderPtr post_processing_ptr; 
TexturePtr noise_texture,
           paper_texture;
        
float background_color[4] = {0, 0, 0, 1};

clock_t frame_timer;
unsigned long frame_time;

bool unlimited_framerate = false;

void Framework::init(std::string title, Vector2 resolution, int window_scale) {
    srand(time(NULL));
    res = resolution;

    // Init
    InitWindow(res.x * window_scale, res.y * window_scale, (title).c_str());
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);

    frame_timer = clock();

    BeginDrawing();
    rlImGuiSetup(true);
    rlImGuiBegin();
    ImGui::SetWindowFontScale(2);
    ImGui::SetWindowCollapsed(true);
    ImGui::SetWindowSize({358, 104});
    rlImGuiEnd();
    EndDrawing();

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
    ClearBackground(Float4ToColor(background_color));

    SceneManager::scene_on->draw_entities(delta);
    DrawableManager::draw();
    
    ColliderManager::draw_debug();
    AreaManager::draw_debug();

    EndMode2D();        
    EndTextureMode();
}

void Framework::draw_ui_layer(float delta) {
    // Ui layer
    BeginTextureMode(ui_layer);
    ClearBackground({0, 0, 0, 0});

    DrawableManager::draw_ui();
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
        frame_timer = clock();
        BeginDrawing();
        
        Framework::draw_game_layer(delta);
        Framework::draw_ui_layer(delta);
        // Compose UI and game layer
        BeginTextureMode(composition_layer);
        DrawTexturePro(game_layer.texture,
            {0, 0, res.x, res.y},
            {0, 0, res.x, res.y},
            {0, 0},
            0,
            WHITE
        );
        DrawTexturePro(ui_layer.texture,
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



        rlImGuiBegin();
        ImVec4 performance_color = frame_time > (1.0/60.0 * 1000) ?
            ImVec4(1, 0, 0, 1) : ImVec4(0, 1, 0, 1);

        ImGui::TextColored(
            performance_color,
            ("FPS: " + std::to_string(GetFPS())).c_str()
        );
        ImGui::TextColored(
            performance_color,
            ("Frame time: " + std::to_string(frame_time) + "ms /16.66ms").c_str()
        );

        bool before = unlimited_framerate;
        ImGui::Checkbox("Unlimited framerate", &unlimited_framerate);
        if (before != unlimited_framerate)
            SetTargetFPS(unlimited_framerate ? 0 : 60);

        ImGui::Text(("Entities: " + std::to_string(
            SceneManager::scene_on->entity_count()
        )).c_str());
        ImGui::Text(("Components: " + std::to_string(
            ComponentManager::component_count()
        )).c_str());

        ImGui::ColorEdit4("Background color", background_color);

        clock_t new_frame_timer = clock();
        frame_time  = new_frame_timer - frame_timer;
        rlImGuiEnd();
        EndDrawing();

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

    rlImGuiShutdown();
    CloseWindow();
}