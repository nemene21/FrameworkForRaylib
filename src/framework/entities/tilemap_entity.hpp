#ifndef TILEMAP_H
#define TILEMAP_H

#include <sprites.hpp>
#include <entity.hpp>

#include <camera_component.hpp>
#include <tilemap_renderer.hpp>
#include <collider_component.hpp>
#include <area_component.hpp>
#include <drawables.hpp>
#include <fstream>
#include <json.hpp>
#include <tuple>

#include <map>
#include <set>

#define PATHFINDING_DIST_MULT 1.2f

typedef struct {
    void* parent;
    int x, y;
    float g_cost, h_cost;
} PathNode;

using json = nlohmann::json;

typedef struct {
    Vector2 pos, state;
} TileData;

typedef std::vector<TileData> TileDataVector;

void to_json(json& j, const TileData& data);
void from_json(const json& j, TileData& data);

void to_json(json& j, const Vector2& vec);
void from_json(const json& j, Vector2& vec);

void to_json(json& j, const TileDataVector& vec);
void from_json(const json& j, TileDataVector& vec);

/// @brief Draws a tilemap and handles autotiling and collision on the TILEMAP collision layer
class Tilemap: public Entity {
private:
    typedef struct {
        int type;
        ColliderComponent collider;
    } TileTypeData;

    int type_count;

    typedef std::map<std::pair<int, int>, TileTypeData> TileTypeMap;
    typedef std::map<std::pair<int, int>, TileTypeMap> TileTypeChunks;

    typedef std::map<std::pair<int, int>, TileDataVector> TileDataChunks;
    typedef std::set<std::pair<int, int>> ChunkSet;

    TexturePtr texture;

    TileTypeChunks tiledata;

    TileDataChunks built_chunks;    
    ChunkSet changed_chunks;

public:
    Vector2 tilesize;
    Vector2 chunksize;
    TilemapRenderer renderer;

    Tilemap(Vector2 tilesize, std::string texture_path);

    void process(float delta);
    void render(float delta);

    /// @brief Set tile at x, y to type
    /// @param x X coord of tile
    /// @param y Y coord of tile
    /// @param type Tile type (0 >=)
    /// @note The position of the tile is in tile position units, not pixels
    void set_tile(int x, int y, int type);
    void set_tile(Vector2 tilepos, int type);
    /// @brief Removes tile at x, y
    /// @param x X coord of tile
    /// @param y Y coord of tile
    void remove_tile(int x, int y);
    void remove_tile(Vector2 tilepos);
    /// @brief Returns tile type at x, y
    /// @param x X coord of tile
    /// @param y Y coord of tile
    /// @return Tile type
    int  get_tile(int x, int y);
    int  get_tile(Vector2 tilepos);

    Vector2 to_tilepos(Vector2 position);

    /// @brief Builds the drawing data and collider data of the tilemap
    void build();
    void build_chunk(std::pair<int, int>);

    /// @brief Returns the farthest point a ray reaches
    /// @param from The starting point
    /// @param to The end point
    Vector2 cast_ray(Vector2 from, Vector2 to);
    /// @brief Returns the direction to move at to get to a certain point
    /// @param from The starting point
    /// @param to The end point
    Vector2 pathfind(Vector2 from, Vector2 to, int max_iterations=1024);

    /// @brief Saves tile data to disk
    /// @param path Path to save on
    void save(std::string path);
    /// @brief Loads tile data from the disk
    /// @param path Path to load from
    void load(std::string path);
};


#endif