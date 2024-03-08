#include <tilemap_entity.hpp>

Tilemap::Tilemap(Vector2 tilesize, std::string texture_path):
    texture {TextureManager::get(texture_path)},
    tiledata {},
    drawable_tiles {},
    changed {false},
    tilesize {tilesize} {}

void Tilemap::set_tile(int x, int y, int type) {
    if (tiledata[std::make_pair(x, y)] == type) {
        return;
    }
    changed = true;
    tiledata[std::make_pair(x, y)] = type;
}

int Tilemap::get_tile(int x, int y) {
    std::pair<int, int> pos = std::make_pair(x, y);

    if (tiledata.find(pos) == tiledata.end())
        return -1;
    
    return tiledata[pos];
}

void Tilemap::build() {
    drawable_tiles.clear();
    changed = false;

    std::set<std::pair<float, float>> corners;
    for (auto& tile: tiledata) {
        std::pair<float, float> pos = std::make_pair<float, float>(tile.first.first, tile.first.second);

        corners.insert({pos.first +.5f, pos.second +.5f});
        corners.insert({pos.first -.5f, pos.second -.5f});
        corners.insert({pos.first +.5f, pos.second -.5f});
        corners.insert({pos.first -.5f, pos.second +.5f});
    }

    for (std::pair<float, float> pos: corners) {
        std::string bitmap = "";

        bitmap += get_tile(pos.first - .5f, pos.second - .5f) == -1 ? "." : "#";
        bitmap += get_tile(pos.first + .5f, pos.second - .5f) == -1 ? "." : "#";
        bitmap += get_tile(pos.first - .5f, pos.second + .5f) == -1 ? "." : "#";
        bitmap += get_tile(pos.first + .5f, pos.second + .5f) == -1 ? "." : "#";

        TileData data {{pos.first, pos.second}, {0, 0}};
    	
        // Corners
        if      (bitmap == "...#")
            data.state = {0, 0};

        else if (bitmap == "..#.")
            data.state = {1, 0};

        else if (bitmap == ".#..")
            data.state = {0, 1};
        
        else if (bitmap == "#...")
            data.state = {1, 1};

        // Edges
        else if (bitmap == "..##")
            data.state = {2, 0};

        else if (bitmap == "#.#.")
            data.state = {3, 0};

        else if (bitmap == ".#.#")
            data.state = {2, 1};
        
        else if (bitmap == "##..")
            data.state = {3, 1};
        
        // Inner corners
        else if (bitmap == "###.")
            data.state = {2, 2};

        else if (bitmap == "##.#")
            data.state = {3, 2};

        else if (bitmap == "#.##")
            data.state = {2, 3};

        else if (bitmap == ".###")
            data.state = {3, 3};

        // Touching corners
        else if (bitmap == ".##.")
            data.state = {0, 2};

        else if (bitmap == "#..#")
            data.state = {1, 2};
        
        // Full block
        else if (bitmap == "####")
            data.state = {0, 3};

        if (bitmap != "....") {
            drawable_tiles.push_back(data);
        }
    }
}

void Tilemap::process(float delta) {}

void Tilemap::draw(float delta) {
    for (auto& tile: drawable_tiles) {

        DrawTextureSheet(texture.get(), tile.state, {4, 4}, {tile.pos.x * tilesize.x, tile.pos.y * tilesize.y}, {1, 1});
        // DrawCircle(tile.pos.x * tilesize.x, tile.pos.y * tilesize.y, 4, RED);
    }
}