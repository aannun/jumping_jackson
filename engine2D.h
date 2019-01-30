#include "libfbxc.h"
#include <glad.h>
#include <SDL.h>
#include <math.h>

typedef struct animation
{
    unsigned int tex_width;
    unsigned int tex_heigth;
    float time;
    float fps;
    unsigned int curr_index;
    unsigned int start_index;
    unsigned int end_index;

}animation_t;

typedef struct sprite
{
    float pivotX;
    float pivotY;
    float posZ;

    GLuint texture;
    unsigned int tex_hash;

    float *uvs;
    animation_t *animation;

} sprite_t;

typedef struct obj2D
{
    float posX;
    float posY;
    float rotation;
    float scale;
    sprite_t *sprite;

    unsigned int collidable;

}obj2D_t;

typedef struct context_2D
{
    GLuint program;

    GLuint mesh;
    unsigned int mesh_len;

    int loaded_textures_length;

    GLint engine_2D_pos_uniform;
    GLint engine_2D_rot_uniform;
    GLint engine_2D_scale_uniform;
    GLint engine_2D_pivot_uniform;

    unsigned int sprites_max_len;
    int sprites_len;
    sprite_t **sprites;

} context_2D_t;

int init_engine2D(context_2D_t *engine_instance, int max_textures_len);
int init_sprite(context_2D_t *engine_instance, sprite_t *sprite, const char *filename, size_t file_len, float posZ);
int init_obj2D(context_2D_t *engine_instance, obj2D_t *obj, const char *filename, size_t file_len, float posX, float posY, float posZ, float scale);
void engine_2D_draw_sprite(context_2D_t *engine, obj2D_t *obj);
void engine_2D_draw_sprites(context_2D_t *engine, obj2D_t *objs, size_t len);
void engine_2D_set_uvs(sprite_t *sprite, float *uvs);

int check_overlap(obj2D_t *obj1, obj2D_t *obj2, float *diffX, float *diffY, float *x1, float *x2, float *y1, float *y2);
int check_collision(obj2D_t *obj1, obj2D_t *obj2);

int init_aimation(sprite_t *sprite, unsigned int width, unsigned int height, float fps, unsigned int start, unsigned int end);
void update_animation(sprite_t *sprite, float delta_time);
void set_anim_info(animation_t *animation, int start, int end, int current, float fps);