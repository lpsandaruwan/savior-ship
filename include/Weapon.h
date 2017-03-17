//
// Created by lpsandaruwan on 3/14/17.
//

#include "Object.h"

#ifndef SAVIOR_WEAPON_H
#define SAVIOR_WEAPON_H


class Weapon: public Object
{
    public:
        // constructors and function definitions
        Weapon(SDL_Rect* clip);
        ~Weapon();

        void fire(const int* SCR_WIDTH, const int* SCR_HEIGHT);

    private:
        int velocityX, velocityY;
};


#endif //SAVIOR_WEAPON_H
