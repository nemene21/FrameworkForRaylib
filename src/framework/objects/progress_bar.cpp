#include <progress_bar.hpp>

ProgressBar::ProgressBar(float width, float height, float value, float maximum):
    UIDrawable(width, height, {0, 0}),
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
    Vector2 pos = Vector2Add(position, offset);
    Vector2 origin = {get_width() * centering.x, get_height() * centering.y};

    DrawRectanglePro({pos.x, pos.y, get_width(), get_height()}, origin, angle, background_clr);
    DrawRectanglePro({pos.x, pos.y, get_width() * ease_progress, get_height()}, origin, angle, ease_clr);
    DrawRectanglePro({pos.x, pos.y, get_width() * progress, get_height()}, origin, angle, progress_clr);
}

void ProgressBar::set_color_scheme(Color background, Color progress, Color ease) {
    background_clr = background;
    progress_clr   = progress;
    ease_clr       = ease;
}
