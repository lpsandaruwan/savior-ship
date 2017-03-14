//
// Created by lpsandaruwan on 3/13/17.
//

#include "Object.h"

#ifndef SAVIOR_COLLISIONCHECK_H
#define SAVIOR_COLLISIONCHECK_H


bool collisionCheck(Object* actor, Object* slave)
{
    // check slave's hit on actor's top corner
    if(actor->Y >= (slave->Y + slave->spriteClip->h))
    {
        return false;
    }

    // check slave's hit on actor's bottom corner
    if((actor->Y + actor->spriteClip->h) <= slave->Y)
    {
        return false;
    }

    // check slave's hit on actor's left side
    if(actor->X >= (slave->X + slave->spriteClip->w))
    {
        return false;
    }

    // check slave's hit on actor's right side
    if(actor->X + actor->spriteClip->w <= slave->X)
    {
        return false;
    }

    return true;
}


#endif //SAVIOR_COLLISIONCHECK_H
