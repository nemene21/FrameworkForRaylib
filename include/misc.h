#ifndef MISC_STUFF_H
#define MISC_STUFF_H

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <map>
#include <functional>


bool operator==(Color first, Color other);

Color Lerp(Color a, Color b, float c);
Vector2 Lerp(Vector2 a, Vector2 b, float c);

float RandF();
float RandF2();

bool TryingToHotReload();

// <Easing functions>

class Easing {
public:
    static std::map<
        std::string, std::function<float(float)>
    > easing_functions;

    static float ease_in_out(float x);
    static float bounce_out(float x);
    static float elastic_out(float x);

    static void InitEasingFuncs();
};

#endif