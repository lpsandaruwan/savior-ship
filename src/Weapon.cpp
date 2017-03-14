//
// Created by lpsandaruwan on 3/14/17.
//

#include "Weapon.h"


Weapon::Weapon(SDL_Rect* clip): Object(clip)
{
    velocityX = 0;
    velocityY = 0;
}

Weapon::~Weapon() {}

void Weapon::fire(const int *SCR_WIDTH, const int *SCR_HEIGHT)
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

    velocityY = -axis_velocity;
}
