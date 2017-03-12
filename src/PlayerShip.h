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
        // class and function definitions
        PlayerShip(int* width, int* height);

        void handleEvent(SDL_Event& e);
        void move(const int* SCR_WIDTH, const int* SCR_HEIGHT);

    private:
        int velocityX, velocityY;
};


#endif //SAVIOR_PLAYERSHIP_H
