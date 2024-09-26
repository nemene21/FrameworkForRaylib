#include <ui_drawables.hpp>

UIDrawable::UIDrawable(float width, float height, Vector2 centering):
    Drawable({0, 0}, {0, 0}, {1, 1}, 0, "-", true),
    width {width},
    height {height},
    centering {centering}
    {}

float UIDrawable::get_width() {
    return width * scale.x;
}
float UIDrawable::get_height() {
    return height * scale.y;
}

Vector2 UIDrawable::real_pos() {
    return {
        (position.x + offset.x) - get_width()  * centering.x,
        (position.y + offset.y) - get_height() * centering.y
    };
}

Vector2 UIDrawable::get_dimensions() {
    return {get_width(), get_height()};
}