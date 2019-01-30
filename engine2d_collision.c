#include "engine2D.h"

#define COLLISION_FALSE 0
#define COLLISION_TRUE 1

int check_overlap(obj2D_t *obj1, obj2D_t *obj2, float *diffX, float *diffY, float *x1, float *x2, float *y1, float *y2)
{
    *x1 = obj1->posX - obj1->sprite->pivotX*obj1->scale;
    *y1 = obj1->posY - obj1->sprite->pivotY*obj1->scale;
    *x2 = obj2->posX - obj2->sprite->pivotX*obj2->scale;
    *y2 = obj2->posY - obj2->sprite->pivotY*obj2->scale;

    /*int x_axis_overlap = COLLISION_FALSE;
    if (x1 + obj1->scale >= x2 && x2 + obj2->scale >= x1)
    {
        *diffX = (x1 + obj1->scale) - x2;
        x_axis_overlap = COLLISION_TRUE;
    }

    int y_axis_overlap = COLLISION_FALSE;
    if (y1 - obj1->scale <= y2 && y2 - obj2->scale <= y1)
    {
        *diffY = (y1 - obj1->scale) - y2;
        y_axis_overlap = COLLISION_TRUE;
    }
    return x_axis_overlap && y_axis_overlap;*/

    if( *x1 < *x2 + obj2->scale && 
        *x1 + obj1->scale > *x2 &&
        *y1 < *y2 + obj2->scale && 
        *y1 + obj1->scale > *y2)
    {
        *diffX = obj1->posX - obj2->posX;
        *diffY = obj1->posY - obj2->posY;
        return 1;
    }
    return 0;
}

int check_collision(obj2D_t *obj1, obj2D_t *obj2)
{
    float diffX;
    float diffY;
    float x1;
    float y1;
    float x2;
    float y2;
    int collision_result = check_overlap(obj1, obj2, &diffX, &diffY, &x1, &x2, &y1, &y2);
    return collision_result;
    
    if (collision_result)
    {
        float precision = 0.01;

        if (fabsf(diffX) < fabsf(diffY))
        {
            if (obj1->posY > obj2->posY)
            {
                obj1->posY = y2 + obj2->scale + obj1->sprite->pivotY*obj1->scale + precision;
                return collision_result;
            }
            else
            {
                obj1->posY = y2 - obj1->sprite->pivotY*obj1->scale - precision;
                return collision_result;
            }
        }
        else
        {
            if (obj1->posX > obj2->posX)
            {
                SDL_Log("a");
                obj1->posX = x2 + obj2->scale + obj1->sprite->pivotX*obj1->scale + precision;
                return collision_result;
            }
            else
            {
                obj1->posX = x2 - obj1->sprite->pivotX*obj1->scale - precision;
                return collision_result;
            }
        }
    }

    return collision_result;
}