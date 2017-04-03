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
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <thread>
#include <unistd.h>

#include "include/PlayerShip.h"
#include "include/TextureWrapper.h"
#include "include/rapidxml_utils.hpp"
#include "include/EnemyShip.h"
#include "include/collisionCheck.h"
#include "include/Weapon.h"
#include "include/Asteroid.h"
#include "include/UFO.h"


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

// object destroy animation
void delayDestroyObject(Object* destroyedObject);

// randomize active objects
void modifyRenderObjectList(Object* renderList[], Object* objectList[], SDL_Event *e);

// free memory on application exit
void close();

// initialize global window and global window renderer
SDL_Window* globalWindow = NULL;
SDL_Renderer* globalRenderer = NULL;
SDL_Surface* surfaceMessage;

// background music
Mix_Music* globalMusic = NULL;

// sound clips
Mix_Chunk *boomSound = NULL;
Mix_Chunk *fireSound = NULL;
Mix_Chunk *startSound = NULL;

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

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

                // initializer mixer library
                if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0)
                {
                    std::cout << "Error loading SDL mixer " << Mix_GetError() << std::endl;
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

// load all the images/audio files
bool prepareMediaFiles()
{
    bool startFlag = false;
    bool status = true;

    // load background media
    if(!backgroundWrapper.loadGraphicsFile(globalRenderer, "./assets/graphics/background.png"))
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

    // load music files
    globalMusic = Mix_LoadMUS("./assets/sounds/airship.mp3");
    if(globalMusic == NULL)
    {
        std::cout << "Failed to load sound file  " << Mix_GetError() << std::endl;
        status = false;
    }

    boomSound = Mix_LoadWAV("./assets/sounds/sfx_lose.ogg");
    if(globalMusic == NULL)
    {
        std::cout << "Failed to load sound file  " << Mix_GetError() << std::endl;
        status = false;
    }

    fireSound = Mix_LoadWAV("./assets/sounds/sfx_laser1.ogg");
    if(globalMusic == NULL)
    {
        std::cout << "Failed to load sound file  " << Mix_GetError() << std::endl;
        status = false;
    }

    startSound = Mix_LoadWAV("./assets/sounds/sfx_zap.ogg");
    if(globalMusic == NULL)
    {
        std::cout << "Failed to load sound file  " << Mix_GetError() << std::endl;
        status = false;
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

// generate a random integer number between two integers
int generateRandomNumber(int min, int max)
{
    std::random_device randomDevice;
    std::mt19937 eng(randomDevice());
    std::uniform_int_distribution<> distr(min, max);

    return distr(eng);
}

// used to display destroyed object image for a while
void delayDestroyObject(Object* destroyedObject)
{
    usleep(100000);
    destroyedObject->inProgress = false;
}

// modify appearing places enemy objects offsets
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

void close()
{
    // free memory on loaded media objects
    globalTextureWrapper.free();
    backgroundWrapper.free();

    // destroy window
    SDL_DestroyRenderer(globalRenderer);
    SDL_DestroyWindow(globalWindow);
    globalWindow = NULL;
    globalRenderer = NULL;

    // quit initialize initialized subsystems
    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
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
            bool startupFlag = false;

            SDL_Event e;

            // background image rendering options
            int scrollingOffset = -1900;

            // player score
            int score = 0;

            // text render options
            SDL_Color whiteColor = {255, 255, 255};

            SDL_Rect healthTextBox;
            healthTextBox.x = 10;
            healthTextBox.y = 10;
            healthTextBox.w = 300;
            healthTextBox.h = 50;

            SDL_Rect scoreTextBox;
            scoreTextBox.x = SCREEN_WIDTH - 300;
            scoreTextBox.y = 10;
            scoreTextBox.w = 300;
            scoreTextBox.h = 50;

            SDL_Rect startupTextBox;
            startupTextBox.x = 10;
            startupTextBox.y = 10;
            startupTextBox.w = 650;
            startupTextBox.h = 60;

            // player ship object
            Object *playerShipObject;
            PlayerShip playerShip(&objectClips[206]);
            playerShipObject = &playerShip;

            // prepare player ship object
            playerShipObject->setAxisVelocity(5);
            playerShipObject->setPosition(650, 650);

            // weapon declaration
            Object* playerGunObject;
            Weapon playerGun(&objectClips[128]);
            playerGunObject = &playerGun;
            playerGunObject->setAxisVelocity(25);
            playerGunObject->setAxisVelocity(25);

            // destroy object
            Object* destroyClipObject;
            EnemyShip destroyClip(&objectClips[145]);
            destroyClipObject = &destroyClip;

            // opposite objects declaration
            Object* objectList[19 + 19 + 4];
            int objectPadding = SCREEN_WIDTH / MAX_OBJECT_INDEX;

            for(int i = 0; i < 19; i++)
            {
                objectList[i] = new EnemyShip(&objectClips[49 + i]);
                objectList[i]->setAxisVelocity(3);
            }

            for(int i = 19; i < 28; i++)
            {
                objectList[i] = new Asteroid(&objectClips[152 + i - 19]);
                objectList[i]->setAxisVelocity(2);
            }

            for(int i = 28; i < 32; i++)
            {
                objectList[i] = new UFO(&objectClips[257 + i - 28]);
                objectList[i]->setAxisVelocity(1);
            }

            // objects to render
            Object *toRenderList[MAX_OBJECT_INDEX];

            // initialize rendering object list
            for(int i = 0; i < MAX_OBJECT_INDEX; i++)
            {
                toRenderList[i] = objectList[generateRandomNumber(0, 31)];
                toRenderList[i]->setPosition((int)(1366 / MAX_OBJECT_INDEX * i + 5), -generateRandomNumber(0, 500));
                toRenderList[i]->inProgress = true;
            }

            // create random object assigning thread
            std::thread objectAssignThread(modifyRenderObjectList, toRenderList, objectList, &e);
            objectAssignThread.detach();

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

                // play background music
                if(Mix_PlayingMusic() == 0)
                {
                    Mix_PlayMusic(globalMusic, -1);
                }

                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        mainLoopFlag = false;
                    }

                    playerShipObject->handleEvent(e);

                    if(e.key.keysym.sym == SDLK_RETURN && e.type == SDL_KEYDOWN)
                    {
                        playerShip.health = 100;
                        score = 0;
                        startupFlag = true;

                        Mix_PlayChannel(-1, startSound, 0);
                    }

                    if(e.key.keysym.sym == SDLK_SPACE && e.type == SDL_KEYDOWN)
                    {
                        playerGunObject->setPosition(playerShipObject->X + playerShip.spriteClip->w / 2, playerShipObject->Y);
                        playerGunObject->inProgress = true;

                        Mix_PlayChannel(-1, fireSound, 0);
                    }
                }

                // render weapon activity
                if(playerGunObject->inProgress)
                {
                    playerGun.fire(&SCREEN_WIDTH, &SCREEN_HEIGHT);
                    globalTextureWrapper.render(globalRenderer, playerGunObject->X , playerGunObject->Y, playerGunObject->spriteClip);
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
                        int originalObjectWidth = toRenderList[i]->spriteClip->w;
                        int originalObjectHeight = toRenderList[i]->spriteClip->h;

                        // set destroy clip object as the current
                        destroyClipObject->setPosition(toRenderList[i]->X, toRenderList[i]->Y);
                        destroyClipObject->setAxisVelocity(toRenderList[i]->axis_velocity);
                        destroyClipObject->inProgress = true;
                        toRenderList[i] = destroyClipObject;

                        // set destroy image clip offset
                        toRenderList[i]->X += (originalObjectWidth / 2) - (toRenderList[i]->spriteClip->w / 2);
                        toRenderList[i]->Y += (originalObjectHeight / 2) - (toRenderList[i]->spriteClip->h / 2);

                        playerGunObject->inProgress = false;
                        playerGunObject->setPosition(playerShipObject->X + playerShip.spriteClip->w / 2, playerShipObject->Y);

                        // delay object destroy, destroy sprite clip object
                        std::thread destroyObjectGraphicsSwapThread(delayDestroyObject, toRenderList[i]);
                        destroyObjectGraphicsSwapThread.detach();

                        Mix_PlayChannel(-1, boomSound, 0);
                        playerShip.health -= 5;
                        score += 5;
                    }

                    // check collision with player weapons
                    if(collisionCheck(playerGunObject, toRenderList[i]))
                    {
                        int originalObjectWidth = toRenderList[i]->spriteClip->w;
                        int originalObjectHeight = toRenderList[i]->spriteClip->h;

                        // set destroy clip object as the current
                        destroyClipObject->setPosition(toRenderList[i]->X, toRenderList[i]->Y);
                        destroyClipObject->setAxisVelocity(toRenderList[i]->axis_velocity);
                        destroyClipObject->inProgress = true;
                        toRenderList[i] = destroyClipObject;

                        // set destroy image clip offset
                        toRenderList[i]->X += (originalObjectWidth / 2) - (toRenderList[i]->spriteClip->w / 2);
                        toRenderList[i]->Y += (originalObjectHeight / 2) - (toRenderList[i]->spriteClip->h / 2);

                        playerGunObject->inProgress = false;
                        playerGunObject->setPosition(playerShipObject->X + playerShip.spriteClip->w / 2, playerShipObject->Y);

                        // delay object destroy, destroy sprite clip object
                        std::thread destroyObjectGraphicsSwapThread(delayDestroyObject, toRenderList[i]);
                        destroyObjectGraphicsSwapThread.detach();

                        Mix_PlayChannel(-1, boomSound, 0);
                        score += 5;
                    }
                }

                // end game if player hits health 0
                if( playerShip.health == 0)
                {
                    startupFlag = false;
                }

                if(startupFlag)
                {
                    // update player health
                    surfaceMessage = TTF_RenderText_Solid(globalFont, ("Health: " + std::to_string(playerShip.health) + "%").c_str(), whiteColor);
                    SDL_Texture* msg = SDL_CreateTextureFromSurface(globalRenderer, surfaceMessage);
                    SDL_RenderCopy(globalRenderer, msg, NULL, &healthTextBox);

                    // update player
                    surfaceMessage = TTF_RenderText_Solid(globalFont, ("SCORE: " + std::to_string(score)).c_str(), whiteColor);
                    SDL_Texture* msg2 = SDL_CreateTextureFromSurface(globalRenderer, surfaceMessage);
                    SDL_RenderCopy(globalRenderer, msg2, NULL, &scoreTextBox);
                }
                else
                {
                    surfaceMessage = TTF_RenderText_Solid(globalFont, "GAME OVER / PRESS ENTER TO START", whiteColor);
                    SDL_Texture* msg = SDL_CreateTextureFromSurface(globalRenderer, surfaceMessage);
                    SDL_RenderCopy(globalRenderer, msg, NULL, &startupTextBox);
                }

                // update window
                SDL_RenderPresent(globalRenderer);
                SDL_Delay(10);
            }
        }
    }

    return 0;
}
