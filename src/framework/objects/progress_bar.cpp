#include <progress_bar.hpp>

ProgressBar::ProgressBar(float width, float height, float value, float maximum):
    Drawable({0, 0}, {0, 0}, {1, 1}, 0, "-", true),
    width {width}, height {height}, value {value}, maximum {maximum}
     {
    progress = value / maximum;
    ease_progress = progress;
}

void ProgressBar::process(float delta) {
    progress = value / maximum;
    ease_progress = Lerpi(ease_progress, progress, 5);
}

void ProgressBar::draw() {
    Vector2 pos = real_pos();
    DrawRectangleV(pos, {width, height}, background_clr);
    DrawRectangleV(pos, {width * ease_progress, height}, ease_clr);
    DrawRectangleV(pos, {width * progress, height}, progress_clr);
}

void ProgressBar::set_color_scheme(Color background, Color progress, Color ease) {
    background_clr = background;
    progress_clr   = progress;
    ease_clr       = ease;
}
