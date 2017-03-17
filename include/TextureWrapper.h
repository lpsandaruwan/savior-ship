//
// Created by lpsandaruwan on 3/10/17.
//

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

#ifndef SAVIOR_TEXTUREWRAPPER_H
#define SAVIOR_TEXTUREWRAPPER_H


class TextureWrapper
{
    public:
        // class and function definitions
        TextureWrapper();
        ~TextureWrapper();

        bool loadGraphicsFile(SDL_Renderer* globalRenderer, std::string path);

        int getHeight();
        int getWidth();

        void free();
        void render(SDL_Renderer* globalRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void setAlpha(Uint8 alpha);
        void setBlendMode(SDL_BlendMode blending);
        void setColors(Uint8 red, Uint8 green, Uint8 blue);

    private:
        SDL_Texture* texture;
        int textureHeight;
        int textureWidth;
};


#endif //SAVIOR_TEXTUREWRAPPER_H
