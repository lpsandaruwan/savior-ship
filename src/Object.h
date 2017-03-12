//
// Created by lpsandaruwan on 3/12/17.
//

#include <SDL.h>

#ifndef SAVIOR_OBJECT_H
#define SAVIOR_OBJECT_H


class Object
{
    public:
        // offsets
        int X, Y;

        // dimensions of an object
        int object_width, object_height;

        // axis velocity of an object
        int axis_velocity;

        // class and function definitions
        Object(int* object_width, int* object_height);

        void setAxisVelocity(int velocity);
        void setPosition(int x, int y);

        virtual void handleEvent(SDL_Event& e);
        virtual void move(const int* SCR_WIDTH, const int* SCR_HEIGHT);
};


#endif //SAVIOR_OBJECT_H
