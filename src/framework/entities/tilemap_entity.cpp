#include <tilemap_entity.hpp>

Tilemap::Tilemap(Vector2 tilesize, std::string texture_path):
    texture {TextureManager::get(texture_path)},
    tiledata {},
    built_chunks {},
    changed_chunks {},
    tilesize {tilesize},
    renderer {[this](float delta) {this->render(delta); }},
    chunksize {16, 16} {
        type_count = (texture.get()->width / tilesize.x) / 4;
        set_name("Tilemap");
    }

// Json to/from for TileData and Vector2
void to_json(json& j, const Vector2& vec) {
    j = json{{"x", vec.x}, {"y", vec.y}};
}

void from_json(const json& j, Vector2& vec) {
    j.at("x").get_to(vec.x);
    j.at("y").get_to(vec.y);
}

void to_json(json& j, const TileData& data) {
    j = json{{"pos", data.pos}, {"state", data.state}};
}

void from_json(const json& j, TileData& data) {
    j.at("pos").get_to(data.pos);
    j.at("state").get_to(data.state);
}

void to_json(json& j, const TileDataVector& arr) {
    j = json::array();
    for (const auto& thing: arr) {
        j.push_back(thing);
    }
}

void from_json(const json& j, TileDataVector& arr) {
    for (const auto& thing: j) {
        arr.push_back(thing.get<TileData>());
    }
}

// Sets tile at x, y (tileposition) to 'type' type
void Tilemap::set_tile(int x, int y, int type) {
    std::pair<int, int> chunk_pos = std::make_pair<int, int>(x / chunksize.x, y / chunksize.y);
    
    // Create tile if it's not found in its chunk
    if (tiledata.find(chunk_pos) == tiledata.end())
        tiledata[chunk_pos] = {};

    // Returns if tile already exists there of that type
    if (get_tile(x, y) == type) {
        return;
    }

    // Mark chunks
    changed_chunks.insert({chunk_pos.first, chunk_pos.second});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second + 0});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first + 0, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second - 0});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second - 1});
    changed_chunks.insert({chunk_pos.first - 0, chunk_pos.second - 1});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second - 1});

    // Build tile collider
    ColliderComponent collider = ColliderComponent(this, {x * tilesize.x, y * tilesize.y}, tilesize.x, tilesize.y);
    collider.set_layer((int)ColliderIndex::TILEMAP, true);
    collider.process(0);

    tiledata[chunk_pos][std::make_pair(x, y)] = {type, collider};
}

void Tilemap::save(std::string path) {
    json j = json::array();

    TileDataVector tiles {};
    for (auto& tile_map_pair: tiledata) {
        for (auto tile_pair: tile_map_pair.second) {
            j.push_back(json{
                {"x", tile_pair.first.first},
                {"y", tile_pair.first.second},
                {"type", tile_pair.second.type}
            });
        }
    }

    std::ofstream file(path);

    if (file.is_open()) {
        file << j.dump(4);
        file.close();

    } else {
        std::cout << "Couldnt open file: " << path << std::endl;
    }
}

void Tilemap::load(std::string path) {
    std::ifstream file(path);
    json j;
    file >> j;
    file.close();

    built_chunks.clear();
    tiledata.clear();

    for (auto& el: j) {
        set_tile((int)el["x"], (int)el["y"], (int)el["type"]);
    }
    build();
}

// Returns tile type at x, y (tileposition), -1 if empty
int Tilemap::get_tile(int x, int y) {
    std::pair<int, int> chunk_pos = std::make_pair<int, int>(x / chunksize.x, y / chunksize.y);
    std::pair<int, int> pos = std::make_pair(x, y);

    if (tiledata.find(chunk_pos) == tiledata.end())
        return -1;
    
    if (tiledata[chunk_pos].find(pos) == tiledata[chunk_pos].end())
        return -1;
    
    return tiledata[chunk_pos][pos].type;
}

// Removes tile at x, y (tileposition)
void Tilemap::remove_tile(int x, int y) {
    std::pair<int, int> chunk_pos = std::make_pair<int, int>(x / chunksize.x, y / chunksize.y);
    
    // Create tile if it's not found in its chunk
    if (tiledata.find(chunk_pos) == tiledata.end())
        return;

    // Returns if tile already exists there of that type
    if (get_tile(x, y) == -1) {
        return;
    }

    // Mark chunks
    changed_chunks.insert({chunk_pos.first, chunk_pos.second});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second + 0});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first + 0, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second - 0});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second - 1});
    changed_chunks.insert({chunk_pos.first - 0, chunk_pos.second - 1});
    changed_chunks.insert({chunk_pos.first - 1, chunk_pos.second + 1});
    changed_chunks.insert({chunk_pos.first + 1, chunk_pos.second - 1});

    tiledata[chunk_pos].erase(std::make_pair(x, y));
}

// Builds all chunks
void Tilemap::build() {
    for (auto &chunk_pos: changed_chunks) {
        build_chunk(chunk_pos);
    }

    changed_chunks.clear();
}

// Makes a chunk builds it's drawing and collider data
void Tilemap::build_chunk(std::pair<int, int> chunk_pos) {

    // Clears the chunk
    if (built_chunks.find(chunk_pos) == built_chunks.end())
        built_chunks[chunk_pos] = {};
    else
        built_chunks[chunk_pos].clear();

    // Generate the corner positions from the tiles
    std::set<std::tuple<float, float, int>> corners;
    for (auto& tile: tiledata[chunk_pos]) {
        std::pair<float, float> pos = std::make_pair<float, float>(tile.first.first, tile.first.second);

        corners.insert({pos.first +.5f, pos.second +.5f, tile.second.type});
        corners.insert({pos.first -.5f, pos.second -.5f, tile.second.type});
        corners.insert({pos.first +.5f, pos.second -.5f, tile.second.type});
        corners.insert({pos.first -.5f, pos.second +.5f, tile.second.type});
    }

    for (std::tuple<float, float, int> corner_data: corners) {
        std::string bitmap = "";
        auto pos = std::make_pair(std::get<0>(corner_data), std::get<1>(corner_data));

        // Builds a string bitmap representation from 4 cornering tiles 
        bitmap += get_tile(pos.first - .5f, pos.second - .5f) == -1 ? "." : "#";
        bitmap += get_tile(pos.first + .5f, pos.second - .5f) == -1 ? "." : "#";
        bitmap += get_tile(pos.first - .5f, pos.second + .5f) == -1 ? "." : "#";
        bitmap += get_tile(pos.first + .5f, pos.second + .5f) == -1 ? "." : "#";

        // Tile's collider, position and texture position data
        ColliderComponent collider = ColliderComponent(this, {pos.first, pos.second}, tilesize.x, tilesize.y);

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

        // Move by 4*type to get the correct image displaying
        int type = std::get<2>(corner_data);
        data.state.x += 4 * type;

        // If tile isn't empty, push it
        if (bitmap != "....") {
            built_chunks[chunk_pos].push_back(data);
        }
    }
}

void Tilemap::process(float delta) {
    if (IsKeyPressed(KEY_ENTER)) {
        save("test.json");
    }
}

// Draws the tilemap (spatial partitioning and camera culling at play)
void Tilemap::render(float delta) {
    Vector2 camera_pos {half_res.x, half_res.y};
    float max_dist = sqrt(tilesize.x*tilesize.x + tilesize.y*tilesize.y) +
        sqrt(res.x*res.x + res.y*res.y);

    // If camera exists set the actual camera pos
    if (CameraManager::get_camera() != nullptr) {
        camera_pos = Vector2Subtract(CameraManager::get_camera()->target, CameraManager::get_camera()->offset);
    }

    // Get chunk position of camera
    std::pair<int, int> camera_chunk_pos = std::make_pair<int, int>(
        (camera_pos.x / tilesize.x) / chunksize.x,
        (camera_pos.y / tilesize.y) / chunksize.y
    );

    // Iterates trough a 3x3 chunk area and attempts drawing the tiles
    for (int x = camera_chunk_pos.first - 1; x <= camera_chunk_pos.first + 1; x++) {
        for (int y = camera_chunk_pos.second - 1; y <= camera_chunk_pos.second + 1; y++) {

            // Pos of chunk that's gonna get drawn
            std::pair<int, int> chunk_pos = std::make_pair(x, y);

            if (built_chunks.find(chunk_pos) != built_chunks.end()) { // If chunk exists
                TileDataVector &drawables = built_chunks[chunk_pos];
                // For all tiles
                for (auto& tile: drawables) {
                    Vector2 tile_pos {tile.pos.x * tilesize.x, tile.pos.y * tilesize.y};

                    // Draw tile if in camera view
                    if (Vector2Distance(camera_pos, tile_pos) < max_dist)
                        DrawTextureSheet(texture.get(), tile.state, {4.f*type_count, 4}, tile_pos, {1, 1});
                    DrawCircleV(tile_pos, 2, RED);
                }
            }
        }
    }
}