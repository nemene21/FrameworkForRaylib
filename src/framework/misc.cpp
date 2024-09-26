#include <misc.hpp>

Vector2 res {320, 180};
Vector2 half_res = Vector2Multiply(res, {.5f, .5f});
float res_diagonal = sqrtf(res.x*res.x + res.y*res.y);

Color Float4ToColor(float* arr) {
    return Color{
        (unsigned char)(arr[0] * 255),
        (unsigned char)(arr[1] * 255),
        (unsigned char)(arr[2] * 255),
        (unsigned char)(arr[3] * 255)
    };
}

int rand32() {
    unsigned int first = rand();
    unsigned int other = rand();
    return (first << 16) | other;
}

const std::string roman_numbers[] = {
    "M" , "CM", "D",
    "CD", "C" , "XC", 
    "L" , "XL", "X" ,
    "IX", "V" , "IV",
    "I"
};
const int numbers[] = {
    1000, 900, 500,
    400 , 100, 90 ,
    50  , 40 , 10 ,
    9   , 5  , 4  ,
    1
};

std::string roman_numeral(int num) {
    for (int i = 0; i < 13; i++) {
        int number_on = numbers[i];
        if (num >= number_on)
            return roman_numbers[i] + roman_numeral(num - number_on);
    }
    return "";
}

bool operator==(Color first, Color other) {
    return first.r == other.r && first.g == other.g && first.b == other.b && first.a == other.a;
}

std::ostream& operator<<(std::ostream& str, Vector2 vec) {
    str << "vec(" << vec.x << ", " << vec.y << ")"; 
    return str;
}

std::ostream& operator<<(std::ostream& str, Color col) {
    str << "col(" << (unsigned int)col.r << ", " << (unsigned int)col.g << ", " << (unsigned int)col.b << ", " << (unsigned int)col.a << ")";
    return str;
}

bool operator>(Vector2 first, Vector2 other) {
    return Vector2Length(first) > Vector2Length(other);
}

bool operator<(Vector2 first, Vector2 other) {
    return Vector2Length(first) < Vector2Length(other);
}

bool operator==(Vector2 first, Vector2 other) {
    return first.x == other.x && first.y == other.y;
}

bool operator!=(Vector2 first, Vector2 other) {
    return first.x != other.x || first.y != other.y;
}

Vector2 operator+(Vector2 first, Vector2 other) {
    return Vector2{first.x + other.x, first.y + other.y};
}

Vector2 operator-(Vector2 first, Vector2 other) {
    return Vector2{first.x - other.x, first.y - other.y};
}

Vector2 operator*(Vector2 first, Vector2 other) {
    return Vector2{first.x * other.x, first.y * other.y};
}

Vector2 operator/(Vector2 first, Vector2 other) {
    return Vector2{first.x / other.x, first.y / other.y};
}

Vector2 operator+=(Vector2& first, Vector2 other) {
    first.x += other.x;
    first.y += other.y;
    return Vector2{first.x, first.y};
}

Vector2 operator-=(Vector2& first, Vector2 other) {
    first.x -= other.x;
    first.y -= other.y;
    return Vector2{first.x, first.y};
}

Vector2 operator*=(Vector2& first, Vector2 other) {
    first.x *= other.x;
    first.y *= other.y;
    return Vector2{first.x, first.y};
}

Vector2 operator/=(Vector2& first, Vector2 other) {
    first.x /= other.x;
    first.y /= other.y;
    return Vector2{first.x, first.y};
}

Vector2 operator+(Vector2 first, float other) {
    return first + Vector2{other, other};
}

Vector2 operator-(Vector2 first, float other) {
    return first + Vector2{other, other};
}

Vector2 operator*(Vector2 first, float other) {
    return first * Vector2{other, other};
}

Vector2 operator/(Vector2 first, float other) {
    return first / Vector2{other, other};
}

Vector2 operator+=(Vector2& first, float other) {
    first.x += other;
    first.y += other;
    return Vector2{first.x, first.y};
}

Vector2 operator-=(Vector2& first, float other) {
    first.x -= other;
    first.y -= other;
    return Vector2{first.x, first.y};
}

Vector2 operator*=(Vector2& first, float other) {
    first.x *= other;
    first.y *= other;
    return Vector2{first.x, first.y};
}

Vector2 operator/=(Vector2& first, float other) {
    first.x /= other;
    first.y /= other;
    return Vector2{first.x, first.y};
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

float Lerpi(float a, float b, float speed) {
    float blend = pow(.5f, GetFrameTime() * speed);

    return Lerp(b, a, blend);
}

Vector2 Lerpi(Vector2 a, Vector2 b, float speed) {
    float blend = pow(.5f, GetFrameTime() * speed);

    return Lerp(b, a, blend);
}

std::string stringify(Vector2 vec) {
    return std::to_string((int)vec.x) + "," + std::to_string((int)vec.y);
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

float Easing::ease_out(float x) {
    return 1.f - (1.f - x) * (1.f - x);
}

float Easing::ease_in(float x) {
    return x * x * x;
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

float Easing::back_in(float x) {
    float const c1 = 1.70158f;
    float const c3 = c1 + 1.f;

    return c3 * x * x * x - c1 * x * x;
}

float Easing::back_out(float x) {
    float const c1 = 1.70158f;
    float const c3 = c1 + 1.f;

    return 1 + c3 * pow(x - 1.f, 3.f) + c1 * pow(x - 1.f, 2.f);
}

float Easing::back_in_out(float x) {
    float const c1 = 1.70158f;
    float const c2 = c1 * 1.525f;

    return x < 0.5f
        ? (pow(2.f * x, 2.f) * ((c2 + 1.f) * 2.f * x - c2)) * .5f
        : (pow(2.f * x - 2.f, 2.f) * ((c2 + 1.f) * (x * 2.f - 2.f) + c2) + 2.f) * .5f;
}

// Initialise ease function map (used for eg. by the particle animation)
void Easing::InitEasingFuncs() {
    easing_functions.emplace("ease_in_out", ease_in_out);
    easing_functions.emplace("ease_out", ease_out);
    easing_functions.emplace("ease_in",  ease_in);

    easing_functions.emplace("bounce_out",  bounce_out);
    easing_functions.emplace("elastic_out", elastic_out);

    easing_functions.emplace("back_in",  back_in);
    easing_functions.emplace("back_out", back_out);
    easing_functions.emplace("back_in_out", back_in_out);
}