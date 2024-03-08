#ifndef TILEMAP_H
#define TILEMAP_H

#include <sprites.hpp>
#include <entity.hpp>

#include <camera_component.hpp>

#include <map>
#include <set>

class Tilemap: public Entity {
private:
    typedef struct {
        Vector2 pos, state;
    } TileData;

    TexturePtr texture;

    std::map<std::pair<int, int>, int> tiledata;
    std::vector<TileData> drawable_tiles;

public:
    bool changed;
    Vector2 tilesize;

    Tilemap(Vector2 tilesize, std::string texture_path);

    void process(float delta);
    void draw(float delta);

    void set_tile(int x, int y, int type);
    int  get_tile(int x, int y);

    void build();
};

#endif