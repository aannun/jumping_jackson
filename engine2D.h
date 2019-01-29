#include "libfbxc.h"
#include <glad.h>
#include <SDL.h>
#include <math.h>

typedef struct sprite
{
    float posX;
    float posY;
    float posZ;
    float rotation;
    float scale;

    GLuint texture;

} sprite_t;

typedef struct context_2D
{
    GLuint program;

    GLuint mesh;
    unsigned int mesh_len;

    int loaded_textures_length;

    GLint engine_2D_pos_uniform;
    GLint engine_2D_rot_uniform;
    GLint engine_2D_scale_uniform;

    unsigned int sprites_max_len;
    int sprites_len;
    sprite_t **sprites;

} context_2D_t;

int init_engine2D(context_2D_t *engine_instance, int max_textures_len);
int init_sprite(context_2D_t *engine_instance, sprite_t *sprite, const char *filename, size_t file_len, float posX, float posY, float posZ);
void engine_2D_draw_sprites(context_2D_t *engine_instance);

int check_overlap(sprite_t *sprite1, sprite_t *sprite2, float *diffX, float *diffY);
int check_collision(sprite_t *sprite1, sprite_t *sprite2);