#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H
#include <ui_drawables.hpp>

/// @brief Draws a progress bar on the UI layer
class ProgressBar: public UIDrawable {
public:
    float width, height, value, maximum, progress, ease_progress;
    Color background_clr, progress_clr, ease_clr;
    ProgressBar(float width, float height, float value, float maximum);

    void process(float delta);
    void draw();
    /// @brief Sets the colors of the progress bar
    /// @param background Background color
    /// @param progress Color of the progress part of the bar
    /// @param ease Color of the animated lagging part of the bar
    void set_color_scheme(Color background, Color progress, Color ease);
};

#endif