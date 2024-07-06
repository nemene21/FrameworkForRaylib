#ifndef TILEMAP_RENDERER_H
#define TILEMAP_RENDERER_H
#include <drawables.hpp>
#include <functional>

class TilemapRenderer: public Drawable {
public:
    TilemapRenderer(std::function<void(float)> func);
    
    std::function<void(float)> func;
    void draw();
};
 
#endif