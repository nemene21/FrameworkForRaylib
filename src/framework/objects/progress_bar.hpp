#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H
#include <ui_drawables.hpp>

class ProgressBar: public UIDrawable {
public:
    float width, height, value, maximum, progress, ease_progress;
    Color background_clr, progress_clr, ease_clr;
    ProgressBar(float width, float height, float value, float maximum);

    void process(float delta);
    void draw();
    void set_color_scheme(Color background, Color progress, Color ease);
};

#endif