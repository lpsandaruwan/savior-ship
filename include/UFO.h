//
// Created by lpsandaruwan on 3/13/17.
//

#include "Object.h"

#ifndef SAVIOR_UFO_H
#define SAVIOR_UFO_H


class UFO: public Object
{
    public:
        // constructors and function definitions
        UFO(SDL_Rect* clip);
        ~UFO();

        void move(const int* SCR_WIDTH, const int* SCR_HEIGHT);
        void setAutoMove(int identifier);

    private:
        int velocityX, velocityY;
};


#endif //SAVIOR_UFO_H
