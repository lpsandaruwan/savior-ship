//
// Created by lpsandaruwan on 3/14/17.
//

#include "../include/Weapon.h"


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

    if((Y < 0) || Y > *SCR_HEIGHT)
    {
        Y = 0;
        inProgress = false;
    }

    velocityY = -axis_velocity;
}
