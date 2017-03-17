//
// Created by lpsandaruwan on 3/10/17.
//

#include "../include/PlayerShip.h"


PlayerShip::PlayerShip(SDL_Rect* clip): Object(clip)
{
    velocityX = 0;
    velocityY = 0;
}

PlayerShip::~PlayerShip() {}

void PlayerShip::move(const int* SCR_WIDTH, const int* SCR_HEIGHT)
{
    // move ship
    X += velocityX;
    Y += velocityY;

    // stop if ship meets frame edges
    if((X < 0) || (X + spriteClip->w) > *SCR_WIDTH)
    {
        X -= velocityX;
    }

    if((Y < 0) || (Y + spriteClip->h) > *SCR_HEIGHT)
    {
        Y -= velocityY;
    }
}

void PlayerShip::handleEvent(SDL_Event &e)
{
    // set key press/release status
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch(e.key.keysym.sym)
        {
            case SDLK_UP:
                velocityY -= axis_velocity;
                break;
            case SDLK_DOWN:
                velocityY += axis_velocity;
                break;
            case SDLK_LEFT:
                velocityX -= axis_velocity;
                break;
            case SDLK_RIGHT:
                velocityX += axis_velocity;
                break;
        }
    }

    if(e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch(e.key.keysym.sym)
        {
            case SDLK_UP:
                velocityY += axis_velocity;
                break;
            case SDLK_DOWN:
                velocityY -= axis_velocity;
                break;
            case SDLK_LEFT:
                velocityX += axis_velocity;
                break;
            case SDLK_RIGHT:
                velocityX -= axis_velocity;
                break;
        }
    }
}
