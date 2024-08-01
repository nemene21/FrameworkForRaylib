#include <iostream>
#include <time.h>
#include <string>

#ifndef WEB
    #include <rich_presence.hpp>
#endif

// Raylib
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

// Imgui
#include <imgui.h>
#include <rlImGui.h>
#include <rlImGuiColors.h>

// Networking
#include <networking/networking.hpp>
#include <networking/packets.hpp>

// Misc
#include <misc.hpp>
#include <input.hpp>

// Component/entity/scene
#include <entity.hpp>
#include <component.hpp>
#include <scene.hpp>

// Components
#include <health_component.hpp>
#include <transform_component.hpp>
#include <camera_component.hpp>
#include <collider_component.hpp>
#include <area_component.hpp>
#include <animation_component.hpp>

// Objects
#include <objects/drawables.hpp>
#include <objects/sprites.hpp>
#include <objects/trails.hpp>
#include <objects/particles.hpp>
#include <objects/audio.hpp>

/// @brief Used to initialise the framework and run it in main.cpp
class Framework {
private:
    static void process_modules(float delta);
    static void process_scene(float delta);
    static void draw_game_layer(float delta);
    static void draw_ui_layer(float delta);
    static void debug_gui();

public:
    static void init(std::string title, Vector2 resolution, int window_scale=1, bool _debug_window=false);
    static void deinit();
    static void run();
};