#include <misc.h>

bool operator==(Color first, Color other) {
    return first.r == other.r && first.g == other.g && first.b == other.b && first.a == other.a;
}

// Lerps
Color Lerp(Color a, Color b, float c) {
    Color color;
    color.r = Lerp(a.r, b.r, c);
    color.g = Lerp(a.g, b.g, c);
    color.b = Lerp(a.b, b.b, c);
    color.a = Lerp(a.a, b.a, c);
    return color;
}

Vector2 Lerp(Vector2 a, Vector2 b, float c) {
    return {
        Lerp(a.x, b.x, c),
        Lerp(a.y, b.y, c)
    };
}

// Random float from 0 to 1
float RandF() {
    return rand() % 1000 * .001f;
}

// Random float from -1 to 1
float RandF2() {
    return RandF() * 2.0f - 1.0f;
}

// Returns true if the hot reload keybind is being pressed
bool TryingToHotReload() {
    return IsKeyPressed(KEY_R) && IsKeyDown(KEY_LEFT_CONTROL);
}

// <Easing functions>
std::map<
    std::string, std::function<float(float)>
> Easing::easing_functions;

float Easing::ease_in_out(float x) {
    return x < .5f ? 2.f * x * x : 1.f - pow(-2.f * x + 2.f, 2.f) * .5f;
}

float Easing::bounce_out(float x) {
    const float n1 = 7.5625f;
    const float d1 = 2.75f;

    if (x < 1 / d1) {
        return n1 * x * x;
    } else if (x < 2 / d1) {
        return n1 * (x -= 1.5f / d1) * x + 0.75f;
    } else if (x < 2.5 / d1) {
        return n1 * (x -= 2.25f / d1) * x + 0.9375f;
    } else {
        return n1 * (x -= 2.625f / d1) * x + 0.984375f;
    }
}

float Easing::elastic_out(float x) {
    const float c4 = (2.0f * PI) * .33f;

    return x == 0.f
    ? 0.f
    : x == 1.f
    ? 1.f
    : pow(2.f, -10.f * x) * sin((x * 10.f - 0.75f) * c4) + 1.f;
}

void Easing::InitEasingFuncs() {
    easing_functions.emplace("ease_in_out", ease_in_out);
    easing_functions.emplace("bounce_out", bounce_out);
    easing_functions.emplace("elastic_out", elastic_out);
}