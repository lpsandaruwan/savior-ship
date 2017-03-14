//
// Created by lpsandaruwan on 3/13/17.
//

#include "Object.h"

#ifndef SAVIOR_ENEMYSHIP_H
#define SAVIOR_ENEMYSHIP_H


class EnemyShip: public Object
{
    public:
        // constructors and function definitions
        EnemyShip(SDL_Rect* clip);
        ~EnemyShip();

        void move(const int* SCR_WIDTH, const int* SCR_HEIGHT);
        void setAutoPilot(int identifier);

    private:
        int velocityX, velocityY;
};


#endif //SAVIOR_ENEMYSHIP_H
