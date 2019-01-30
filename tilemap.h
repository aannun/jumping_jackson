#include "engine2D.h"
typedef struct TileMap
{
    int *tileSet;
    unsigned int width;
    unsigned int height;
    const char *filename;
    unsigned int map_width;
    unsigned int map_height;
}Tile_map_t;

int TileMapInit(Tile_map_t *Tile_map, unsigned int width, unsigned int height, int *tileSet, const char *filename, unsigned int map_width, unsigned int map_height);
void *CreateLevel(context_2D_t *engine, Tile_map_t *tileMap, float offsetX, float offsetY, float posZ, float scale, int *out_len, int *collidable_indexes, int indexes_len);