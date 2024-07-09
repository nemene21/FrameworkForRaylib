#include <drawables.hpp>

class UIDrawable: public Drawable {
public:
    Vector2 centering;
    float width, height;
    UIDrawable(float width, float height, Vector2 centering={0, 0});

    float get_width();
    float get_height();
    Vector2 get_dimensions();
    Vector2 real_pos();
};