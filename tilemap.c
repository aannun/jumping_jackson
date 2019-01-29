#include "tilemap.h"
#include "stdio.h"

int tileContextInit(tileContext_t *ctx, unsigned int tile_lenght, const char **filename)
{
    memset(ctx, 0, sizeof(tileContext_t));
    ctx->filename = filename;
    ctx->tile_lenght = tile_lenght;
    return 1;
}

int TileMapInit(Tile_map_t *Tile_map, float width, float height, int *tileSet)
{
    Tile_map->width = width;
    Tile_map->height = height;
    Tile_map->tileSet = tileSet;
    return 1;
}

int CreateLevel(tileContext_t *ctx, Tile_map_t *tileMap, context_2D_t *engine, float scale)
{
    int size = tileMap->height * tileMap->width;
    int i = 0;

    for(i = 0; i < size; i++)
    {
        sprite_t *sprite = malloc(sizeof(sprite_t));

        int index = tileMap->tileSet[i];
        if(index == -1 || index >= ctx->tile_lenght)
            continue;
        const char *name = ctx->filename[tileMap->tileSet[i]];
        init_sprite(engine,sprite,name,strlen(name),(i%tileMap->width) - (2-(tileMap->width*scale)/2), (i/tileMap->width) - (2-(tileMap->height*scale)/2), 0);
        sprite->scale = scale;
    }
    return 1;
}