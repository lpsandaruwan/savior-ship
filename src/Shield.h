//
// Created by lpsandaruwan on 3/13/17.
//

#include "Object.h"

#ifndef SAVIOR_SHIELD_H
#define SAVIOR_SHIELD_H


class Shield: Object
{
    public:
        // constructors and function definitions
        Shield(SDL_Rect* clip);
        ~Shield();

        void move(const int* SCR_WIDTH, const int* SCR_HEIGHT);
        void setAutoMove(int identifier);

    private:
        int velocityX, velocityY;
};


#endif //SAVIOR_SHIELD_H
