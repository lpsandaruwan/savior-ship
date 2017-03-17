//
// Created by lpsandaruwan on 3/13/17.
//

#include "../include/Shield.h"


Shield::Shield(SDL_Rect* clip): Object(clip)
{
    velocityX = 0;
    velocityY = 0;
}

Shield::~Shield() {}

void Shield::move(const int *SCR_WIDTH, const int *SCR_HEIGHT)
{
    // move asteroid
    X += velocityX;
    Y += velocityY;

    // stop if ship meets frame edges
    if((X + spriteClip->w * 2 < 0) || (X - spriteClip->w) > *SCR_WIDTH)
    {
        X -= velocityX;
    }

    if((Y + spriteClip->h * 2 < 0) || (Y - spriteClip->h) > *SCR_HEIGHT)
    {
        Y -= velocityY;
    }
}

void Shield::setAutoMove(int identifier)
{

}
