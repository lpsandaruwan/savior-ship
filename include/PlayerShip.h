//
// Created by lpsandaruwan on 3/10/17.
//

#include <SDL.h>
#include "TextureWrapper.h"
#include "Object.h"

#ifndef SAVIOR_PLAYERSHIP_H
#define SAVIOR_PLAYERSHIP_H


class PlayerShip: public Object
{
    public:
        int health = 100;

        // constructors and function definitions
        PlayerShip(SDL_Rect* clip);
        ~PlayerShip();

        void move(const int* SCR_WIDTH, const int* SCR_HEIGHT);
        void handleEvent(SDL_Event& e);

    private:
        int velocityX, velocityY;
};


#endif //SAVIOR_PLAYERSHIP_H
