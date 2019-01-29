#include "engine2D.h"

#define COLLISION_FALSE 0
#define COLLISION_TRUE 1

int check_overlap(sprite_t *sprite1, sprite_t *sprite2, float *diffX, float *diffY)
{
    float x1 = sprite1->posX - sprite1->scale / 2;
    float y1 = sprite1->posY + sprite1->scale / 2;
    float x2 = sprite2->posX - sprite2->scale / 2;
    float y2 = sprite2->posY + sprite2->scale / 2;

    int x_axis_overlap = COLLISION_FALSE;
    if (x1 + sprite1->scale <= x2)
    {
        *diffX = (x1 + sprite1->scale) - x2;
        x_axis_overlap = COLLISION_TRUE;
    }

    if (x2 + sprite2->scale <= x1)
    {
        *diffX = (x2 + sprite2->scale) - x1;
        x_axis_overlap = COLLISION_TRUE;
    }

    int y_axis_overlap = COLLISION_FALSE;
    if (y1 - sprite1->scale >= y2)
    {
        *diffY = (y1 - sprite1->scale) - y2;
        y_axis_overlap = COLLISION_TRUE;
    }

    if(y2 - sprite2->scale >= y1)
    {
        *diffY = (y2 - sprite2->scale) - y1;
        y_axis_overlap = COLLISION_TRUE;
    }

    return x_axis_overlap && y_axis_overlap;
}

int check_collision(sprite_t *sprite1, sprite_t *sprite2)
{
    float diffX;
    float diffY;
    int collision_result = check_overlap(sprite1, sprite2, &diffX, &diffY);
    if (collision_result)
    {
        float precision = 0.01;
        float scale1 = sprite1->scale / 2;
        float scale2 = sprite2->scale / 2;
        if (fabsf(diffX) > fabsf(diffY))
        {
            if (sprite1->posY < sprite2->posY)
            {
                SDL_Log("alto");
                sprite2->posX = sprite1->posX + scale1 + scale2 + precision;
                return collision_result;
            }
            else
            {
                //OK
                SDL_Log("basso");
                sprite2->posY = sprite1->posY - scale1 - scale2 - precision;
                return collision_result;
            }
        }
        else
        {
            if (sprite1->posX < sprite2->posX)
            {
                //OK
                SDL_Log("destra");
                sprite2->posX = sprite1->posX + scale1 + scale2 + precision;
                return collision_result;
            }
            else
            {
                SDL_Log("sinistra");
                sprite2->posX = sprite1->posX - scale1 - scale2 - precision;
                return collision_result;
            }
        }
    }

    return collision_result;
}