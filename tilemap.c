#include "tilemap.h"
#include "stdio.h"

int TileMapInit(Tile_map_t *Tile_map, unsigned int width, unsigned int height, int *tileSet, const char *filename, unsigned int map_width, unsigned int map_height)
{
    Tile_map->width = width;
    Tile_map->height = height;
    Tile_map->tileSet = tileSet;
    Tile_map->filename = filename;
    Tile_map->map_width = map_width;
    Tile_map->map_height = map_height;
    return 1;
}

static int contains_index(int *indexes, int len, int index)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (indexes[i] == index)
            return 1;
    }
    return 0;
}

void *CreateLevel(context_2D_t *engine, Tile_map_t *tileMap, float offsetX, float offsetY, float posZ, float scale, int *out_len, int *collidable_indexes, int indexes_len)
{
    int size = tileMap->height * tileMap->width;
    int i = 0;
    obj2D_t *objs = malloc(sizeof(obj2D_t) * size);
    int count = 0;

    float uvs[12];

    for (i = 0; i < size; i++)
    {
        int index = tileMap->tileSet[i];
        if (index < 0)
            continue;
            
        const char *name = tileMap->filename;
        init_obj2D(engine, &objs[count], name, strlen(name), (i % tileMap->width) * scale + offsetX, (1 - (i / tileMap->width) * scale) + offsetY, posZ, scale);

        float sizeX = 1 / (float)tileMap->map_width;
        float sizeY = 1 / (float)tileMap->map_height;
        float x = (float)((int)(index % tileMap->map_width)) * sizeX;
        float y = (float)((int)(index / tileMap->map_width)) * sizeY;

        uvs[0] = x + sizeX;
        uvs[1] = y;

        uvs[2] = x;
        uvs[3] = y;

        uvs[4] = x;
        uvs[5] = y + sizeY;

        uvs[6] = x;
        uvs[7] = y + sizeY;

        uvs[8] = x + sizeX;
        uvs[9] = y + sizeY;

        uvs[10] = x + sizeX;
        uvs[11] = y;

        engine_2D_set_uvs(objs[count].sprite, uvs);

        if (contains_index(collidable_indexes, indexes_len, index))
            objs[count].collidable = 1;

        count++;
    }
    
    *out_len = count;        
    return objs;
}