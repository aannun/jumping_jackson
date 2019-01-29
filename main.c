#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "tilemap.h"

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

    context_2D_t engine;
    if (!init_engine2D(&engine, 256))
    {
        free(&engine);
        return -1;
    }

    tileContext_t tileMap;
    const char *name = "Texture.TGA";
    const char *files = {name};
    tileContextInit(&tileMap, 1, &files);

    int tileM[] = {-1, 0, 0, 0, 0,
                   0, -1, 0, 0, 0,
                   0, 0, -1, 0, 0,
                   0, 0, 2, -1, 0,
                   0, 0, 0, 0, -1};

    Tile_map_t TileImplementation;
    TileMapInit(&TileImplementation, 5, 5, tileM);
    CreateLevel(&tileMap, &TileImplementation, &engine, 0.2);

    sprite_t sprites[10];
    int max_sprites = 10;
    int sprite_len = 0;
    int curr_sprite = 0;

    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RIGHT)
                    sprites[curr_sprite].posX += 0.1;
                if (event.key.keysym.sym == SDLK_LEFT)
                    sprites[curr_sprite].posX -= 0.1;
                if (event.key.keysym.sym == SDLK_UP)
                    sprites[curr_sprite].posY += 0.1;
                if (event.key.keysym.sym == SDLK_DOWN)
                    sprites[curr_sprite].posY -= 0.1;
                if (event.key.keysym.sym == SDLK_r)
                    sprites[curr_sprite].rotation += 0.1;
                if (event.key.keysym.sym == SDLK_e)
                    sprites[curr_sprite].rotation -= 0.1;
                if (event.key.keysym.sym == SDLK_f)
                    curr_sprite = (curr_sprite + 1) % sprite_len;
                if (event.key.keysym.sym == SDLK_g)
                {
                    init_sprite(&engine, &sprites[sprite_len], "Texture.TGA", 11, 0, 0, 0);
                    sprites[sprite_len].scale = 0.3;
                    sprite_len = (sprite_len+1)%max_sprites;
                    curr_sprite = sprite_len-1;
                    printf_s("%d", curr_sprite);
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        engine_2D_draw_sprites(&engine);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}