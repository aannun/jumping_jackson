#include "engine2D.h"

#define COLLISION_FALSE 0
#define COLLISION_TRUE 1

int check_collision(sprite_t *sprite1, sprite_t *sprite2)
{
    float x1 = sprite1->posX - sprite1->scale / 2;
    float y1 = sprite1->posY + sprite1->scale / 2;
    float x2 = sprite2->posX - sprite2->scale / 2;
    float y2 = sprite2->posY + sprite2->scale / 2;

    int x_axis_overlap = COLLISION_FALSE;
    if (x1 + sprite1->scale >= x2 && x2 + sprite2->scale >= x1)
    {
        x_axis_overlap = COLLISION_TRUE;
    }

    int y_axis_overlap = COLLISION_FALSE;
    if (y1 - sprite1->scale <= y2 && y2 - sprite2->scale <= y1)
    {
        y_axis_overlap = COLLISION_TRUE;
    }

    return x_axis_overlap && y_axis_overlap;
}