#include "engine2D.h"
typedef struct TileMap
{
    int *tileSet;
    unsigned int width;
    unsigned int height;
}Tile_map_t;

typedef struct TileContext
{
    const char **filename;
    unsigned int tile_lenght;
}tileContext_t;


int tileContextInit(tileContext_t *ctx, unsigned int tile_lenght, const char **filename);
int TileMapInit(Tile_map_t *Tile_map, float width, float height, int *tileSet);
int CreateLevel(tileContext_t *ctx, Tile_map_t *tileMap, context_2D_t *engine, float scale);