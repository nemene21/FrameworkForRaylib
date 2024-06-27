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

typedef struct {
    float x, y, radius;
} Circle;

bool operator==(Color first, Color other);

std::ostream& operator<<(std::ostream& str, Vector2 vec);
std::ostream& operator<<(std::ostream& str, Color col);

bool operator>(Vector2 first, Vector2 other);
bool operator<(Vector2 first, Vector2 other);
bool operator==(Vector2 first, Vector2 other);

/// @brief Linear interpolation between two colors
/// @param a Color 1
/// @param b Color 2
/// @param c Ratio
/// @return Interpolated color
Color Lerp(Color a, Color b, float c);
/// @brief Linear interpolation between two vectors
/// @param a Vector 1
/// @param b Vector 2
/// @param c Ratio
/// @return Interpolated vector
Vector2 Lerp(Vector2 a, Vector2 b, float c);

/// @brief Frame independent lerp (uses delta)
/// @param a First value
/// @param b Second value
/// @param speed 
/// @return Interpolated value
float Lerpi(float a, float b, float speed);
/// @brief Frame independent lerp for vectors (uses delta)
/// @param a First vector
/// @param b Second vector
/// @param speed 
/// @return Interpolated vector
Vector2 Lerpi(Vector2 a, Vector2 b, float speed);

std::string stringify(Vector2 vec);

/// @brief Random float 0 : 1
/// @return Random float 0 : 1
float RandF();

/// @brief Random float -1 : 1
/// @return Random float -1 : 1
float RandF2();

/// @brief Returns true if the hotkey for hot reloading is pressed (ctrl + r)
bool TryingToHotReload();

/// @brief Class that holds cool easing functions for animation
/// @note All of these functions expect a number x between 0 and 1 and return a number between 0 and 1 aswell
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