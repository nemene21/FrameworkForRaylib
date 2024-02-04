
#include <misc.h>

bool operator==(Color first, Color other) {
    return first.r == other.r && first.g == other.g && first.b == other.b && first.a == other.a;
}

Color Lerp(Color a, Color b, float c) {
    Color color;
    color.r = Lerp(a.r, b.r, c);
    color.g = Lerp(a.g, b.g, c);
    color.b = Lerp(a.b, b.b, c);
    color.a = Lerp(a.a, b.a, c);
    return color;
}