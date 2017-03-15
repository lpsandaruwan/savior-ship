/***
 * SAVIOR SHIP- Figther spaceship game using SDL2 libraries.
 * Just for fun. Please enjoy...!!!
 *
 * savior-ship is a free game: you can redistribute it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * Savior is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * See COPYING for a copy of the GNU General Public License. If not, see http://www.gnu.org/licenses/.
 *
 * @author Lahiru Pathirage <lpsandaruwan@gmail.com> 10/03/2017
 ***/

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <thread>
#include <unistd.h>

#include "src/PlayerShip.h"
#include "src/TextureWrapper.h"
#include "src/rapidxml_utils.hpp"
#include "src/EnemyShip.h"
#include "src/collisionCheck.h"
#include "src/Weapon.h"
#include "src/Asteroid.h"
#include "src/UFO.h"


// screen size
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

// maximum number of opposite objects to render
const int MAX_OBJECT_INDEX = 6;

// create main window
bool initialize();

// prepare media files
bool prepareMediaFiles();

// generate a random number within a range
int generateRandomNumber(int min, int max);

void modifyRenderObjectList(Object* renderList[], Object* objectList[], SDL_Event *e);

// free memory on application exit
void close();

// initialize global window and global window renderer
SDL_Window* globalWindow = NULL;
SDL_Renderer* globalRenderer = NULL;
SDL_Surface* surfaceMessage;

// initialize font
TTF_Font* globalFont = NULL;

// assets paths
std::string spriteSheet = "./assets/graphics/sheet.png";
const char* spriteSheetInfo = "./assets/graphics/sheet.xml";

// sprite clips
SDL_Rect objectClips[294];

// initialize global texture renderer and background renderer
TextureWrapper backgroundWrapper;
TextureWrapper globalTextureWrapper;

bool initialize()
{
    bool status = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error initializing SDL " << SDL_GetError() << std::endl;
    }
    else
    {
        // set texture filtering to linear mode
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            std::cout << "Error enabling linear texture filtering" << std::endl;
        }

        // create global window
        globalWindow = SDL_CreateWindow("SAVIOR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if(globalWindow == NULL)
        {
            std::cout << "Error creating main window " << SDL_GetError() << std::endl;
            status = false;
        }
        else
        {
            globalRenderer = SDL_CreateRenderer(globalWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

            if(globalRenderer == NULL)
            {
                std::cout << "Error creating global renderer " << SDL_GetError() << std::endl;
                status = false;
            }
            else
            {
                SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // initialize PNG loading
                int imageFlags = IMG_INIT_PNG;

                if(!(IMG_Init(imageFlags) & imageFlags))
                {
                    std::cout << "Error loading SDL image " << IMG_GetError() << std::endl;
                    status = false;
                }

                // initialize ttf library
                if(TTF_Init() == -1)
                {
                    std::cout << "Error loading SDL ttf " << TTF_GetError() << std::endl;
                    status = false;
                }
            }
        }
    }

    return status;
}

bool prepareMediaFiles()
{
    bool status = true;

    // load background media
    if(!backgroundWrapper.loadGraphicsFile(globalRenderer, "./assets/graphics/background.jpg"))
    {
        std::cout << "Failed to load background image " << std::endl;
        status = false;
    }

    if(!globalTextureWrapper.loadGraphicsFile(globalRenderer, spriteSheet))
    {
        std::cout << "Failed to load image " << std::endl;
        status = false;
    }
    else
    {
        // read from sprite sheet info file
        rapidxml::xml_document<> doc;

        std::ifstream infoFile(spriteSheetInfo);
        std::stringstream buffer;
        buffer << infoFile.rdbuf();
        infoFile.close();

        std::string content(buffer.str());
        doc.parse<0>(&content[0]);

        rapidxml::xml_node<> *pRoot = doc.first_node();

        // iterate through nodes and put attributes in sprite sheet list
        int i = 0;
        for(rapidxml::xml_node<> *pNode=pRoot->first_node("SubTexture"); pNode; pNode = pNode->next_sibling())
        {
            objectClips[i].x = atoi(pNode->first_attribute("x")->value());
            objectClips[i].y = atoi(pNode->first_attribute("y")->value());
            objectClips[i].w = atoi(pNode->first_attribute("width")->value());
            objectClips[i].h = atoi(pNode->first_attribute("height")->value());

            i++;
        }
    }

    // load font file
    globalFont = TTF_OpenFont("./assets/fonts/kenvector_future_thin.ttf", 10);

    if(globalFont == NULL)
    {
        std::cout << "Failed to load font file  " << TTF_GetError() << std::endl;
        status = false;
    }

    return status;
}

void modifyRenderObjectList(Object* renderList[], Object* objectList[], SDL_Event* e)
{
    while(true)
    {
        if(e->type == SDL_QUIT)
        {
            break;
        }

        usleep(100);

        for(int i = 0; i < MAX_OBJECT_INDEX; i++)
        {
            if(!renderList[i]->inProgress)
            {
                renderList[i] = objectList[generateRandomNumber(0, 31)];
                renderList[i]->setPosition((int)(1366 / MAX_OBJECT_INDEX * i + 5), -generateRandomNumber(0, 500));

                renderList[i]->inProgress = true;
            }
        }
    }
}

int generateRandomNumber(int min, int max)
{
    std::random_device randomDevice;
    std::mt19937 eng(randomDevice());
    std::uniform_int_distribution<> distr(min, max);

    return distr(eng);
}

int main(int argc, char* args[])
{
    if(!initialize())
    {
        printf("Failed to initialize");
    }
    else
    {
        if(!prepareMediaFiles())
        {
            std::cout << "Failed to load media files " << std::endl;
        }
        else
        {
            bool mainLoopFlag = true;

            SDL_Event e;

            // background image rendering options
            int scrollingOffset = -1900;

            // text render options
            SDL_Color whiteColor = {255, 255, 255};
            SDL_Rect textBox;
            textBox.x = 10;
            textBox.y = 10;
            textBox.w = 300;
            textBox.h = 50;

            // player ship object
            Object *playerShipObject;
            PlayerShip playerShip(&objectClips[206]);
            playerShipObject = &playerShip;

            // prepare player ship object
            playerShipObject->setAxisVelocity(5);
            playerShipObject->setPosition(650, 650);

            // weapon declaration
            Object* leftGunObject;
            Object* rightGunObject;
            Weapon leftGun(&objectClips[128]);
            Weapon rightGun(&objectClips[128]);
            leftGunObject = &leftGun;
            rightGunObject = &rightGun;
            leftGunObject->setAxisVelocity(25);
            rightGunObject->setAxisVelocity(25);

            // opposite objects declaration
            Object* objectList[19 + 19 + 4];
            int objectPadding = SCREEN_WIDTH / MAX_OBJECT_INDEX;

            for(int i = 0; i < 19; i++)
            {
                objectList[i] = new EnemyShip(&objectClips[49 + i]);
                objectList[i]->setAxisVelocity(2);
                objectList[i]->setPosition(objectPadding * (i % MAX_OBJECT_INDEX), -generateRandomNumber(0, 500));
            }

            for(int i = 19; i < 28; i++)
            {
                objectList[i] = new Asteroid(&objectClips[152 + i - 19]);
                objectList[i]->setAxisVelocity(1);
                objectList[i]->setPosition(objectPadding * (i % MAX_OBJECT_INDEX), -generateRandomNumber(0, 500));
            }

            for(int i = 28; i < 32; i++)
            {
                objectList[i] = new UFO(&objectClips[257 + i - 28]);
                objectList[i]->setAxisVelocity(1);
                objectList[i]->setPosition(objectPadding * (i % MAX_OBJECT_INDEX), -generateRandomNumber(0, 500));
            }

            // objects to render
            Object *toRenderList[MAX_OBJECT_INDEX];

            // initialize rendering object list
            for(int i = 0; i < MAX_OBJECT_INDEX; i++)
            {
                toRenderList[i] = objectList[generateRandomNumber(0, 31)];
                toRenderList[i]->setAxisVelocity(2);
                toRenderList[i]->inProgress = true;
            }

            // create random object assigning thread
            std::thread t(modifyRenderObjectList, toRenderList, objectList, &e);
            t.detach();

            while(mainLoopFlag)
            {
                // scroll background
                ++scrollingOffset;

                if(scrollingOffset > backgroundWrapper.getWidth())
                {
                    scrollingOffset = -1900;
                }

                // clear screen
                SDL_SetRenderDrawColor(globalRenderer, 0, 0, 0, 0);
                SDL_RenderClear(globalRenderer);

                // render background
                backgroundWrapper.render(globalRenderer, 0, scrollingOffset);
                backgroundWrapper.render(globalRenderer, 0, scrollingOffset - backgroundWrapper.getWidth());

                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        mainLoopFlag = false;
                    }

                    playerShipObject->handleEvent(e);

                    if(e.key.keysym.sym == SDLK_SPACE && e.key.repeat == 0 && e.type == SDL_KEYDOWN)
                    {
                        leftGunObject->setPosition(playerShipObject->X, playerShipObject->Y);
                        rightGunObject->setPosition(playerShipObject->X, playerShipObject->Y);

                        leftGunObject->inProgress = true;
                        rightGunObject->inProgress = true;
                    }
                }

                // render weapon activity
                if(leftGunObject->inProgress)
                {
                    leftGun.fire(&SCREEN_WIDTH, &SCREEN_HEIGHT);
                    globalTextureWrapper.render(globalRenderer, leftGunObject->X + 10, leftGunObject->Y, leftGunObject->spriteClip);
                }

                if(rightGunObject->inProgress)
                {
                    rightGun.fire(&SCREEN_WIDTH, &SCREEN_HEIGHT);
                    globalTextureWrapper.render(globalRenderer, rightGunObject->X + playerShip.spriteClip->w - 20, rightGunObject->Y, rightGunObject->spriteClip);
                }

                // render player ship
                playerShipObject->move(&SCREEN_WIDTH, &SCREEN_HEIGHT);
                globalTextureWrapper.render(globalRenderer, playerShip.X, playerShip.Y, playerShip.spriteClip);

                // add other objects to render list
                for(int i = 0; i < MAX_OBJECT_INDEX; i++)
                {
                    toRenderList[i]->move(&SCREEN_WIDTH, &SCREEN_HEIGHT);
                    toRenderList[i]->setAutoMove(1);

                    globalTextureWrapper.render(globalRenderer, toRenderList[i]->X, toRenderList[i]->Y, toRenderList[i]->spriteClip, 180);

                    // collision check with player ship
                    if(collisionCheck(playerShipObject, toRenderList[i]))
                    {
                        toRenderList[i]->inProgress = false;
                        playerShip.health -= 5;
                    }

                    // check collision with player weapons
                    if(collisionCheck(leftGunObject, toRenderList[i]))
                    {
                        leftGunObject->inProgress = false;
                        toRenderList[i]->inProgress = false;
                    }

                    if(collisionCheck(rightGunObject, toRenderList[i]))
                    {
                        rightGunObject->inProgress = false;
                        toRenderList[i]->inProgress = false;
                    }
                }

                // update player health
                surfaceMessage = TTF_RenderText_Solid(globalFont, ("Health: " + std::to_string(playerShip.health) + "%").c_str(), whiteColor);
                SDL_Texture* msg = SDL_CreateTextureFromSurface(globalRenderer, surfaceMessage);
                SDL_RenderCopy(globalRenderer, msg, NULL, &textBox);

                // update window
                SDL_RenderPresent(globalRenderer);
            }
        }
    }

    return 0;
}
