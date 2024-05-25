#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <set>
#include <map>

#define MOUSE_KEY + 2024

extern std::map<std::string, std::set<int>> inputs;

bool IsPressed(std::string name);
bool IsJustPressed(std::string name);

#endif