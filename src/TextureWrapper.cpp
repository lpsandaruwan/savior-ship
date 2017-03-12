//
// Created by lpsandaruwan on 3/10/17.
//

#include <iostream>
#include "TextureWrapper.h"


TextureWrapper::TextureWrapper()
{
    texture = NULL;
    textureHeight = 0;
    textureWidth = 0;
}

TextureWrapper::~TextureWrapper()
{
    free();
}

bool TextureWrapper::loadGraphicsFile(SDL_Renderer* globalRenderer, std::string path)
{
    // get rid of preexisting texture
    free();

    // final texture
    SDL_Texture* newTexture = NULL;

    // load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if(loadedSurface == NULL)
    {
        std::cout << "Error while loading image " << IMG_GetError() << std::endl;
    }
    else
    {
        // set color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);
        if(newTexture == NULL)
        {
            std::cout << "Error while creating texture from " << path.c_str() << " " << SDL_GetError() << std::endl;
        }
        else
        {
            // get image dimensions
            textureHeight = loadedSurface->h;
            textureWidth = loadedSurface->w;
        }

        // get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    texture = newTexture;
    return texture != NULL;
}

void TextureWrapper::free()
{
    if(texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
        textureHeight = 0;
        textureWidth = 0;
    }
}

int TextureWrapper::getHeight()
{
    return textureHeight;
}

int TextureWrapper::getWidth()
{
    return textureWidth;
}

void TextureWrapper::setColors(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod(texture, red, green, blue);
}

void TextureWrapper::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(texture, blending);
}

void TextureWrapper::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(texture, alpha);
}

void TextureWrapper::render(SDL_Renderer* globalRenderer, int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    SDL_Rect rendering = {x, y, textureWidth, textureHeight};

    if(clip != NULL)
    {
        rendering.h = clip->h;
        rendering.w = clip->w;
    }

    // render to display
    SDL_RenderCopyEx(globalRenderer, texture, clip, &rendering, angle, center, flip);
}
