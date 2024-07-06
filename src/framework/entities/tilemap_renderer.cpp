#include <tilemap_renderer.hpp>

TilemapRenderer::TilemapRenderer(std::function<void(float)> func): Drawable(), func {func} {}

void TilemapRenderer::draw() {
    func(GetFrameTime());
}