#ifndef MISC_STUFF_H
#define MISC_STUFF_H

#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <map>
#include <string>
#include <functional>

extern const Vector2 res;
extern const Vector2 half_res;

struct Vector2Hash {
    std::size_t operator()(const Vector2& v) const;
};

bool operator==(Color first, Color other);

std::ostream& operator<<(std::ostream& str, Vector2 vec);

bool operator>(Vector2 first, Vector2 other);
bool operator<(Vector2 first, Vector2 other);
bool operator==(Vector2 first, Vector2 other);

Color Lerp(Color a, Color b, float c);
Vector2 Lerp(Vector2 a, Vector2 b, float c);

Vector2 Lerpi(Vector2 a, Vector2 b, float speed);

std::string stringify(Vector2 vec);

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
    static float ease_in (float x);
    static float ease_out(float x);

    static float back_in (float x);
    static float back_out(float x);
    static float back_in_out(float x);
    
    static float bounce_out (float x);
    static float elastic_out(float x);

    static void InitEasingFuncs();
};

#endif