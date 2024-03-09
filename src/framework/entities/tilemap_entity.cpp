#include <tilemap_entity.hpp>

Tilemap::Tilemap(Vector2 tilesize, std::string texture_path):
    texture {TextureManager::get(texture_path)},
    tiledata {},
    built_chunks {},
    changed_chunks {},
    tilesize {tilesize},
    chunksize {16, 16} {}

void Tilemap::set_tile(int x, int y, int type) {
    std::pair<int, int> chunk_pos = std::make_pair<int, int>(x / chunksize.x, y / chunksize.y);

    if (tiledata.find(chunk_pos) == tiledata.end())
        tiledata[chunk_pos] = {};

    if (get_tile(x, y) == type) {
        return;
    }
    changed_chunks.insert({chunk_pos.first, chunk_pos.second});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second + 0});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first + 0, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second - 0});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second - 1});
    changed_chunks.insert({chunk_pos.first - 0, chunk_pos.second - 1});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second - 1});

    tiledata[chunk_pos][std::make_pair(x, y)] = type;
}

int Tilemap::get_tile(int x, int y) {
    std::pair<int, int> chunk_pos = std::make_pair<int, int>(x / chunksize.x, y / chunksize.y);
    std::pair<int, int> pos = std::make_pair(x, y);

    if (tiledata.find(chunk_pos) == tiledata.end())
        return -1;
    
    if (tiledata[chunk_pos].find(pos) == tiledata[chunk_pos].end())
        return -1;
    
    return tiledata[chunk_pos][pos];
}

void Tilemap::build() {
    for (auto& chunk_pos: changed_chunks) {
        build_chunk(chunk_pos);
    }

    changed_chunks.clear();
}

void Tilemap::build_chunk(std::pair<int, int> chunk_pos) {

    if (built_chunks.find(chunk_pos) == built_chunks.end())
        built_chunks[chunk_pos] = {};
    else
        built_chunks[chunk_pos].clear();

    std::set<std::pair<float, float>> corners;
    for (auto& tile: tiledata[chunk_pos]) {
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
            built_chunks[chunk_pos].push_back(data);
        }
    }
}

void Tilemap::process(float delta) {}

void Tilemap::draw(float delta) {
    Vector2 camera_pos {0, 0};
    float max_dist = sqrt(tilesize.x*tilesize.x + tilesize.y*tilesize.y) +
        sqrt(res.x*res.x + res.y*res.y);

    if (CameraManager::get_camera() != nullptr) {
        camera_pos = Vector2Add(CameraManager::get_camera()->target, CameraManager::get_camera()->offset);
        camera_pos = Vector2Subtract(camera_pos, half_res);
    }
    std::pair<int, int> camera_chunk_pos = std::make_pair<int, int>(
        (camera_pos.x / 96.f) / chunksize.x,
        (camera_pos.y / 96.f) / chunksize.y
    );

    for (int x = camera_chunk_pos.first - 2; x < camera_chunk_pos.second + 2; x++) {
        for (int y = camera_chunk_pos.second - 2; y < camera_chunk_pos.second + 2; y++) {

            std::pair<int, int> chunk_pos = std::make_pair(x, y);

            if (built_chunks.find(chunk_pos) != built_chunks.end()) {
                TileDataVector &drawables = built_chunks[chunk_pos];

                for (auto& tile: drawables) {

                    Vector2 tile_pos {tile.pos.x * tilesize.x, tile.pos.y * tilesize.y};

                    if (Vector2Distance(camera_pos, tile_pos) < max_dist)
                        DrawTextureSheet(texture.get(), tile.state, {4, 4}, tile_pos, {1, 1});
                }
            }
        }
    }
}