#include <input.hpp>

// Input map (holds all inputs)
std::map<std::string, std::vector<Input>> inputs = {
    {"jump", {{InputType::KEY, KEY_SPACE}}},
    {"shoot", {{InputType::MOUSE, MOUSE_BUTTON_LEFT}}},
    {"continue", {{InputType::KEY, KEY_SPACE}, {InputType::KEY, KEY_ENTER}, {InputType::KEY, KEY_C}}},

    {"up",    {{InputType::KEY, KEY_W}, {InputType::KEY, KEY_UP   }}},
    {"left",  {{InputType::KEY, KEY_A}, {InputType::KEY, KEY_LEFT }}},
    {"down",  {{InputType::KEY, KEY_S}, {InputType::KEY, KEY_DOWN }}},
    {"right", {{InputType::KEY, KEY_D}, {InputType::KEY, KEY_RIGHT}}}
};

// Check if an input is being held right now
bool IsPressed(std::string name, int gamepad) {
    for (Input input: inputs[name]) {
        if (input.type == InputType::KEY      && IsKeyDown(input.id))                    return true;
        if (input.type == InputType::MOUSE    && IsMouseButtonDown(input.id))            return true;
        if (input.type == InputType::JOYSTICK && IsGamepadButtonDown(gamepad, input.id)) return true;
    }
    return false;
}

// Check if an input was pressed this frame
bool IsJustPressed(std::string name, int gamepad) {
    for (Input input: inputs[name]) {
        if (input.type == InputType::KEY      && IsKeyPressed(input.id))                    return true;
        if (input.type == InputType::MOUSE    && IsMouseButtonPressed(input.id))            return true;
        if (input.type == InputType::JOYSTICK && IsGamepadButtonPressed(gamepad, input.id)) return true;
    }
    return false;
}
// Check if an input was released this frame
bool IsJustReleased(std::string name, int gamepad) {
    for (Input input: inputs[name]) {
        if (input.type == InputType::KEY      && IsKeyReleased(input.id))                    return true;
        if (input.type == InputType::MOUSE    && IsMouseButtonReleased(input.id))            return true;
        if (input.type == InputType::JOYSTICK && IsGamepadButtonReleased(gamepad, input.id)) return true;
    }
    return false;
}

// Get input vector
Vector2 InputVector(std::string left, std::string right, std::string up, std::string down) {
    return {
        (float)IsPressed(right) - (float)IsPressed(left),
        (float)IsPressed(down)  - (float)IsPressed(up)
    };
}

Vector2 InputVectorNormalized(std::string left, std::string right, std::string up, std::string down) {
    return Vector2Normalize(InputVector(left, right, up, down));
}

// Mouse position in world
Vector2 mouse_screen_pos() {
    float draw_width  = GetScreenWidth();
    float draw_height = GetScreenWidth() * (res.y / res.x);

    if (draw_height > GetScreenHeight()) {
        draw_height = GetScreenHeight();
        draw_width  = GetScreenHeight() * (res.x / res.y);
    }

    Vector2 pos = GetMousePosition();
    pos.x = (pos.x / draw_width ) * res.x;
    pos.y = (pos.y / draw_height) * res.y;

    pos.x -= ((GetScreenWidth () - draw_width ) * .5f) / draw_width  * res.x;
    pos.y -= ((GetScreenHeight() - draw_height) * .5f) / draw_height * res.y;
    return pos;
}

Vector2 mouse_pos() {
    Vector2 pos = mouse_screen_pos();
    Camera2D *camera = CameraManager::get_camera();

    pos = Vector2Add(pos, Vector2Subtract(camera->target, camera->offset));

    return pos;
}