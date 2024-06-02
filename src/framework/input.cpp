#include <input.hpp>

// Input map (holds all inputs)
std::map<std::string, std::vector<Input>> inputs = {
    {"jump", {{InputType::KEY, KEY_SPACE}}},
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

// Mouse position in world
Vector2 mouse_pos() {
    Vector2 pos = GetMousePosition();
    pos.x = (pos.x / GetScreenWidth() ) * res.x;
    pos.y = (pos.y / GetScreenHeight()) * res.y;
    return pos;
}