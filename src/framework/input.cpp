#include <input.hpp>

std::map<std::string, std::set<int>> inputs = {
    {"jump", {KEY_SPACE}},
    {"continue", {KEY_SPACE, KEY_ENTER, KEY_C}},

    {"up",    {KEY_W, KEY_UP}},
    {"left",  {KEY_A, KEY_LEFT}},
    {"down",  {KEY_S, KEY_DOWN}},
    {"right", {KEY_D, KEY_RIGHT}}
};


bool IsPressed(std::string name) {
    for (int key: inputs[name]) {
        if (IsKeyDown(key)) return true;
    }
    return false;
}

bool IsJustPressed(std::string name) {
    for (int key: inputs[name]) {
        if (IsKeyPressed(key)) return true;
    }
    return false;
}