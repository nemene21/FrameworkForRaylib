#ifndef TILEMAP_H
#define TILEMAP_H

#include <sprites.hpp>
#include <entity.hpp>

#include <camera_component.hpp>
#include <collider_component.hpp>
#include <area_component.hpp>
#include <fstream>
#include <json.hpp>
#include <tuple>

#include <map>
#include <set>

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

    Tilemap(Vector2 tilesize, std::string texture_path);

    void process(float delta);
    void draw(float delta);

    void set_tile(int x, int y, int type);
    void remove_tile(int x, int y);
    int  get_tile(int x, int y);

    void build();
    void build_chunk(std::pair<int, int>);

    void save(std::string path);
    void load(std::string path);
};

#endif