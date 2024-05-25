#ifndef TILEMAP_H
#define TILEMAP_H

#include <sprites.hpp>
#include <entity.hpp>

#include <camera_component.hpp>
#include <collider_component.hpp>
#include <area_component.hpp>

#include <map>
#include <set>

class Tilemap: public Entity {
private:
    typedef struct {
        Vector2 pos, state;
    } TileData;

    typedef struct {
        int type;
        ColliderComponent collider;
    } TileTypeData;

    typedef std::map<std::pair<int, int>, TileTypeData> TileTypeMap;
    typedef std::map<std::pair<int, int>, TileTypeMap> TileTypeChunks;

    typedef std::vector<TileData> TileDataVector;
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
};

#endif