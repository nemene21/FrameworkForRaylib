#include <framework.hpp>

RenderTexture2D ui_layer, game_layer, composition_layer;

ShaderPtr post_processing_ptr; 
TexturePtr noise_texture;

float background_color[4] = {0, 0, 0, 1};

clock_t frame_timer;
unsigned long frame_time;

std::string local_ip;

bool unlimited_framerate = false;
bool debug_ui;

void Framework::init(std::string title, Vector2 resolution, int window_scale, bool _debug_window) {
    srand(time(NULL));

    res = resolution;
    half_res = Vector2Divide(res, {2, 2});
    res_diagonal = sqrtf(res.x*res.x + res.y*res.y);

    debug_ui = _debug_window;

    #ifndef WEB
        presence_title = title;
        RichPresence::init();

    #endif

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
    SetTextureFilter(ui_layer.texture,          TEXTURE_FILTER_POINT);
    SetTextureFilter(game_layer.texture,        TEXTURE_FILTER_POINT);
    SetTextureFilter(composition_layer.texture, TEXTURE_FILTER_POINT);
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
    SceneManager::init();

    // Loading post processing shader and it's textures
    post_processing_ptr = ShaderManager::get("post_processing.glsl"); 
    noise_texture = TextureManager::get("post_processing/noise.png");

    // Networking init
    local_ip = Networking::get_local_ip();

    Networking::init();
    Packets::init();
}

void Framework::debug_gui() {
    rlImGuiBegin();
    ImVec4 performance_color = frame_time > (1.0/60.0 * 1000) ?
        ImVec4(1, 0, 0, 1) : ImVec4(0, 1, 0, 1);

    ImGui::TextColored(
        performance_color,
        ("FPS: " + std::to_string(GetFPS())).c_str()
    );
    ImGui::TextColored(
        performance_color,
        ("Frame time: " + std::to_string(frame_time) + "ms/16.66ms").c_str()
    );

    bool before = unlimited_framerate;
    ImGui::Checkbox("Unlimited framerate", &unlimited_framerate);
    if (before != unlimited_framerate)
        SetTargetFPS(unlimited_framerate ? 0 : 60);
    
    ImGui::Text(("Local ip: " + local_ip).c_str());

    ImGui::Text(("Entity count: " + std::to_string(
        SceneManager::scene_on->entity_count()
    )).c_str());
    ImGui::Text(("Component count: " + std::to_string(
        ComponentManager::component_count()
    )).c_str());

    if (ImGui::CollapsingHeader("Entities:")) {
        ImGui::Indent(25.f);
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.7f, 1.0f, 0.15f));

        int i = 0;
        for (Entity* entity: SceneManager::scene_on->get_entities()) {
            i++;
            std::string ent_name = entity->get_name().c_str();

            if (ImGui::CollapsingHeader(ent_name.c_str())) {
                ImGui::Indent(25.f);
                ImGui::Text(("Owned: " + (entity->owned ? std::string("true") : std::string("false"))).c_str());

                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.4f, 0.7f, 1.0f, 0.1f));

                if (ImGui::CollapsingHeader(("Components##" + std::to_string(i)).c_str())) {
                    ImGui::Indent(25.f);
                    for (auto component: entity->get_components()) {
                        component->draw_gui_info();
                    }
                    ImGui::Unindent(25.f);
                }
                if (ImGui::CollapsingHeader(("Groups##" + std::to_string(i)).c_str())) {
                    ImGui::Indent(25.f);
                    for (auto group: entity->get_groups()) {
                        ImGui::Text(group.c_str());
                    }
                    ImGui::Unindent(25.f);
                }
                ImGui::Unindent(25.f);
                ImGui::PopStyleColor();
            }
        }
        ImGui::Unindent(25.f);
        ImGui::PopStyleColor();
    }

    ImGui::Checkbox("Draw areas", &DRAW_AREAS);
    ImGui::Checkbox("Draw colliders", &DRAW_COLLIDERS);

    ImGui::ColorEdit4("Background color", background_color);
    rlImGuiEnd();
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

    #ifndef WEB
        RichPresence::update();
    #endif

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

        Shader post_processing = *post_processing_ptr.get();
        BeginShaderMode(post_processing);

        DrawTexturePro(game_layer.texture,
            {0, 0, res.x, res.y},
            {0, 0, res.x, res.y},
            {0, 0},
            0,
            WHITE
        );
        EndShaderMode();

        DrawTexturePro(ui_layer.texture,
            {0, 0, res.x, res.y},
            {0, 0, res.x, res.y},
            {0, 0},
            0,
            WHITE
        );
        EndTextureMode();

        // Post processing uniforms
        float timer = GetTime();
        float aspect_ratio = res.x / res.y;

        SetShaderValue(post_processing, GetShaderLocation(post_processing, "time"), &timer, SHADER_UNIFORM_FLOAT);
        SetShaderValue(post_processing, GetShaderLocation(post_processing, "aspect_ratio"), &aspect_ratio, SHADER_UNIFORM_FLOAT);
        
        // Post processing texture binding
        SetShaderValueTexture(post_processing, GetShaderLocation(post_processing, "noise_texture"), *noise_texture.get());

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
            WHITE
        );

        if (debug_ui) {
            Framework::debug_gui();
        }
        clock_t new_frame_timer = clock();
        frame_time  = new_frame_timer - frame_timer;
        EndDrawing();

    }
    deinit();
}

void Framework::deinit() {
    if (Networking::active()) {
        Networking::disconnect();
    }
    Networking::deinit();
    
    // Unload remaining assets
    TextureManager::unload_all();
    ParticleDataManager::unload_all();
    ShaderManager::unload_all();
    SceneManager::unload_all();
    AudioManager::unload_all();

    #ifndef WEB
        RichPresence::deinit();

    #endif

    rlImGuiShutdown();
    CloseWindow();
}