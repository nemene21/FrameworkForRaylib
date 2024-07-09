#include <drawables.hpp>

/// @brief UI drawables get drawn on their own layer on top of the game layer
class UIDrawable: public Drawable {
public:
    /// @brief 0, 0 for no centering; 0.5, 0.5 for centering
    Vector2 centering;
    float width, height;
    UIDrawable(float width, float height, Vector2 centering={0, 0});

    /// @brief Returns width of the UIDrawable taking into account scale
    /// @return Width of UIDrawable
    float get_width();
    /// @brief Returns height of the UIDrawable taking into account scale
    /// @return Height of UIDrawable
    float get_height();
    /// @brief Returns dimensions of the UIDrawable taking into account scale
    /// @return Dimensions of UIDrawable
    Vector2 get_dimensions();
    /// @brief Returns position of the UIDrawable taking into account centering
    /// @return Position of UIDrawable
    Vector2 real_pos();
};