//
// Created by lpsandaruwan on 3/12/17.
//

#include <SDL.h>

#ifndef SAVIOR_OBJECT_H
#define SAVIOR_OBJECT_H


class Object
{
    public:
        // destroy when conditions met
        bool destroyFlag = false;

        // offsets
        int X, Y;

        // axis velocity of an object
        int axis_velocity;

        // object sprite clip
        SDL_Rect* spriteClip;

        // constructors and function definitions
        Object(SDL_Rect* clip);
        virtual ~Object();

        void setAxisVelocity(int velocity);
        void setPosition(int x, int y);

        virtual void handleEvent(SDL_Event& e);
        virtual void move(const int* SCR_WIDTH, const int* SCR_HEIGHT);
};


#endif //SAVIOR_OBJECT_H
