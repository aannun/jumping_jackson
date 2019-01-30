#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "tilemap.h"

float old_posX;
float old_posY;
int curr_dir;

static int inputs(SDL_Event event, obj2D_t *objs, int *curr_obj, int *objs_len, int max_objs, context_2D_t *engine, const char *fileName)
{
    old_posX = objs[*curr_obj].posX;
    old_posY = objs[*curr_obj].posY;
    int pressed = 0;

    if (event.type == SDL_KEYDOWN)
    {
        if (objs_len > 0)
        {
            if (event.key.keysym.sym == SDLK_RIGHT)
            {
                if (curr_dir != 0)
                {
                    set_anim_info(objs[*curr_obj].sprite->animation, 12, 16, 12, 1 / (float)8);
                    curr_dir = 0;
                }
                objs[*curr_obj].posX += 0.05;
                pressed = 1;
            }
            if (event.key.keysym.sym == SDLK_LEFT)
            {
                if (curr_dir != 1)
                {
                    set_anim_info(objs[*curr_obj].sprite->animation, 8, 12, 8, 1 / (float)8);
                    curr_dir = 1;
                }
                objs[*curr_obj].posX -= 0.05;
                pressed = 1;
            }
            if (event.key.keysym.sym == SDLK_UP)
            {
                if (curr_dir != 2)
                {
                    set_anim_info(objs[*curr_obj].sprite->animation, 4, 8, 4, 1 / (float)8);
                    curr_dir = 2;
                }
                objs[*curr_obj].posY += 0.05;
                pressed = 1;
            }
            if (event.key.keysym.sym == SDLK_DOWN)
            {
                if (curr_dir != 3)
                {
                    set_anim_info(objs[*curr_obj].sprite->animation, 0, 4, 0, 1 / (float)8);
                    curr_dir = 3;
                }
                objs[*curr_obj].posY -= 0.05;
                pressed = 1;
            }
            if (event.key.keysym.sym == SDLK_r)
                objs[*curr_obj].rotation += 0.1;
            if (event.key.keysym.sym == SDLK_e)
                objs[*curr_obj].rotation -= 0.1;
            if (event.key.keysym.sym == SDLK_f)
                *curr_obj = (*curr_obj + 1) % *objs_len;
        }
        if (event.key.keysym.sym == SDLK_g && *objs_len < max_objs)
        {
            init_obj2D(engine, &objs[*objs_len], fileName, strlen(fileName), -0.2, 0, *objs_len, 0.3);
            init_aimation(objs[*objs_len].sprite, 4, 4, 1 / (float)8, 12, 16);
            curr_dir = 0;

            objs[*objs_len].sprite->pivotX = 0.5;
            objs[*objs_len].sprite->pivotY = 0.5;

            *curr_obj = *objs_len;
            *objs_len = *objs_len + 1;
        }
    }

    return pressed;
}

void manage_collisions(obj2D_t *player, obj2D_t *level, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        if (level[i].collidable)
        {
            if (check_collision(player, &level[i]))
            {
                player->posX = old_posX;
                player->posY = old_posY;
            }
        }
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_OPENGL);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // create opengl context with the major/minor specified with SetAttribute
    SDL_GLContext context = SDL_GL_CreateContext(window);

    // load GL symbols
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    // set the current clear color
    glClearColor(0, 0, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    context_2D_t engine;
    if (!init_engine2D(&engine, 256))
    {
        free(&engine);
        return -1;
    }

    int tileM[] = {39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
                   39, -1, -1, -1, -1, 39, -1, 37, -1, 39,
                   39, 36, -1, -1, -1, -1, -1, -1, -1, 39,
                   39, -1, -1, 3, -1, -1, 3, -1, -1, 39,
                   39, -1, -1, -1, 36, -1, -1, -1, -1, 39,
                   39, 39, -1, -1, -1, -1, -1, -1, 37, 39,
                   39, -1, -1, -1, -1, -1, -1, -1, -1, 39,
                   39, -1, 37, -1, 3, -1, -1, -1, -1, 39,
                   39, -1, -1, -1, -1, -1, 36, -1, 39, 39,
                   39, 39, 39, 39, 39, 39, 39, 39, 39, 39};

    Tile_map_t tileMap;
    TileMapInit(&tileMap, 10, 10, tileM, "Textures/tilemap.png", 12, 12);

    int collidable[] = {39, 37, 3};
    int coll_len = 2;

    int len = 0;
    obj2D_t *objs = CreateLevel(&engine, &tileMap, -1, -0.2, 90, 0.2, &len, collidable, coll_len);

    obj2D_t players[2];
    int max_objs = 2;
    int objs_len = 0;
    int curr_obj = 0;

    Uint32 old_time = SDL_GetTicks();
    float delta_time = 0;
    float conversion = 1000;

    curr_dir = 0;
    int pressed = 0;

    obj2D_t back;
    const char *back_name = "Textures/Green.png";
    init_obj2D(&engine, &back, back_name, strlen(back_name), -1, -1, 91, 2);

    for (;;)
    {
        delta_time = (float)(SDL_GetTicks() - old_time) / conversion;
        old_time = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;
            pressed = inputs(event, players, &curr_obj, &objs_len, max_objs, &engine, "Textures/spritesheet.png");
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (objs_len > 0)
        {
            manage_collisions(&players[0], objs, len);
            update_animation(players[0].sprite, delta_time * pressed);

            SDL_Log("%d", players[0].sprite->animation->curr_index);
        }

        engine_2D_draw_sprite(&engine, &back);
        engine_2D_draw_sprites(&engine, objs, len);
        engine_2D_draw_sprites(&engine, players, objs_len);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}