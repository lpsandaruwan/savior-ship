//
// Created by lpsandaruwan on 3/12/17.
//

#include "Object.h"


Object::Object(SDL_Rect* clip)
{
    spriteClip = clip;
}

Object::~Object() {}

void Object::setAxisVelocity(int velocity)
{
    axis_velocity = velocity;
}

void Object::setPosition(int x, int y)
{
    X = x;
    Y = y;
}

void Object::handleEvent(SDL_Event &e) {}
void Object::move(const int *SCR_WIDTH, const int *SCR_HEIGHT) {}
