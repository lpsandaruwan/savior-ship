//
// Created by lpsandaruwan on 3/13/17.
//

#include "Object.h"

#ifndef SAVIOR_ASTEROID_H
#define SAVIOR_ASTEROID_H


class Asteroid: public Object
{
    public:
        // constructors and function definitions
        Asteroid(SDL_Rect* clip);
        ~Asteroid();

        void move(const int* SCR_WIDTH, const int* SCR_HEIGHT);
        void setAutoMove(int identifier);

    private:
        int velocityX, velocityY;
};


#endif //SAVIOR_ASTEROID_H
