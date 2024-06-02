#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <map>
#include <misc.hpp>

enum InputType {
    KEY,
    MOUSE,
    JOYSTICK,
};

typedef struct {
    int type, id;
} Input;

extern std::map<std::string, std::vector<Input>> inputs;

bool IsPressed(std::string name, int gamepad=0);
bool IsJustPressed(std::string name, int gamepad=0);

Vector2 mouse_pos();

#endif