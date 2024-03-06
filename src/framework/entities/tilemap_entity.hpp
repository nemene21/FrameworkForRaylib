#ifndef TILEMAP_H
#define TILEMAP_H

#include <framework.h>
#include <map>
#include <set>

class Tilemap: public Entity {
private:
    Vector2 tilesize;

    typedef struct {
        Vector2 pos, state;
    } TileData;

    TexturePtr texture;

    std::map<std::pair<int, int>, int> tiledata;
    std::vector<TileData> drawable_tiles;

public:
    Tilemap(Vector2 tilesize, std::string texture_path);

    void process(float delta);
    void draw(float delta);

    void set_tile(int x, int y, int type);
    int  get_tile(int x, int y);

    void build();
};

#endif