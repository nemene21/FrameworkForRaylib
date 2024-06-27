#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <map>
#include <misc.hpp>
#include <camera_component.hpp>

enum InputType {
    KEY,
    MOUSE,
    JOYSTICK,
};

typedef struct {
    int type, id;
} Input;

extern std::map<std::string, std::vector<Input>> inputs;

/// @brief Checks if a certain action is held down this frame
/// @param name Name of action
/// @param gamepad Controller to check for input (0 by default)
/// @return True if it's pressed
bool IsPressed(std::string name, int gamepad=0);
/// @brief Checks if a certain action is just pressed this frame
/// @param name Name of action
/// @param gamepad Controller to check for input (0 by default)
/// @return True if it's just pressed
bool IsJustPressed(std::string name, int gamepad=0);
/// @brief Checks if a certain action is just released this frame
/// @param name Name of action
/// @param gamepad Controller to check for input (0 by default)
/// @return True if it's just released
bool IsJustReleased(std::string name, int gamepad=0);

/// @brief Returns a vector based on 4-directional input
/// @param left Left action name
/// @param right Right action name
/// @param up Up action name
/// @param down Down action name
/// @return Vector which turns to the direction of the input
Vector2 InputVector(std::string left, std::string right, std::string up, std::string down);
/// @brief Returns a normalized vector based on 4-directional input
/// @param left Left action name
/// @param right Right action name
/// @param up Up action name
/// @param down Down action name
/// @return Normalized vector which turns to the direction of the input
Vector2 InputVectorNormalized(std::string left, std::string right, std::string up, std::string down);

/// @brief Returns position of the mouse on the screen
/// @return Position of the mouse on the screen
Vector2 mouse_screen_pos();
/// @brief Returns position of the mouse in the world
/// @return Position of the mouse in the world
Vector2 mouse_pos();

#endif