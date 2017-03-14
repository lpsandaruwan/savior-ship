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
#include <iostream>
#include <string>
#include <sstream>
#include <random>

#include "src/PlayerShip.h"
#include "src/TextureWrapper.h"
#include "src/rapidxml_utils.hpp"
#include "src/EnemyShip.h"
#include "src/collisionCheck.h"
#include "src/Weapon.h"


// screen size
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;


// create main window
bool initialize();

// prepare media files
bool prepareMediaFiles();

int generateRandomNumber(int min, int max);

// free memory on application exit
void close();

// initialize global window and global window renderer
SDL_Window* globalWindow = NULL;
SDL_Renderer* globalRenderer = NULL;

// assets paths
std::string spriteSheet = "./assets/graphics/sheet.png";
const char* spriteSheetInfo = "./assets/graphics/sheet.xml";

// sprite clips
SDL_Rect objectClips[294];

// initialize global texture renderer
TextureWrapper globalTextureWrapper;

bool initialize()
{
    bool status = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Error initializing SDL " << SDL_GetError() << std::endl;
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
        }
    }

    return status;
}

bool prepareMediaFiles()
{
    bool status = true;

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

    return status;
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

            // player ship object
            Object *playerShipObject;
            PlayerShip playerShip(&objectClips[206]);
            playerShipObject = &playerShip;

            // prepare player ship object
            playerShipObject->setAxisVelocity(5);
            playerShipObject->setPosition(650, 650);

            // random object generation




            while(mainLoopFlag)
            {
                // clear screen
                SDL_SetRenderDrawColor(globalRenderer, 0, 0, 0, 0);
                SDL_RenderClear(globalRenderer);

                playerShipObject->move(&SCREEN_WIDTH, &SCREEN_HEIGHT);

                while(SDL_PollEvent(&e) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        mainLoopFlag = false;
                    }

                    playerShipObject->handleEvent(e);

                    if(e.key.keysym.sym == SDLK_SPACE && e.key.repeat == 0 && e.type == SDL_KEYDOWN)
                    {

                    }
                }

                // render objects
                globalTextureWrapper.render(globalRenderer, playerShip.X, playerShip.Y, playerShip.spriteClip);

                // update window
                SDL_RenderPresent(globalRenderer);
            }
        }
    }

    return 0;
}
