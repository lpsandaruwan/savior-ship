//
// Created by lpsandaruwan on 3/13/17.
//

#include "UFO.h"


UFO::UFO(SDL_Rect* clip): Object(clip)
{
    velocityX = 0;
    velocityY = 0;
}

UFO::~UFO() {}

void UFO::move(const int *SCR_WIDTH, const int *SCR_HEIGHT)
{
    // move ship
    X += velocityX;
    Y += velocityY;

    // stop if ship meets frame edges
    if((X < 0) || (X - spriteClip->w) > *SCR_WIDTH)
    {
        X -= velocityX;
    }

    if((Y < 0) || (Y - spriteClip->h) > *SCR_HEIGHT)
    {
        Y -= velocityY;
    }
}

void UFO::setAutoPilot(int identifier)
{
    // identifier 0 -set X axis velocity, 1 -set Y axis velocity, 2 -set both
    switch(identifier)
    {
        case 0:
            velocityX += axis_velocity;
            break;
        case 1:
            velocityY += axis_velocity;
            break;
        case 2:
            velocityX += axis_velocity;
            velocityY += axis_velocity;
            break;
        default:
            velocityX -= axis_velocity;
            velocityY -= axis_velocity;
            break;
    }
}
