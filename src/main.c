#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"

#define SDL_ASSERT_LEVEL 2

SDL_Texture *LoadTexture1(SDL_Renderer *renderer, char *path)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path);

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s Error: %s\n", path, IMG_GetError());
        return NULL;
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == NULL)
        {
            printf("Unable to create texture from %s Error: %s\n", path, IMG_GetError());
            return NULL;
        }
        else
        {
            SDL_FreeSurface(loadedSurface);
            loadedSurface = NULL;
        }
    }
    return texture;
}

SDL_Texture *LoadTexture2(SDL_Renderer *renderer, char *path)
{
    SDL_Texture *texture = NULL;

    texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL)
    {
        printf("Unable to create texture from %s Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    return texture;
}

int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Start\n");

    int iGameWidth = 1024, iGameHeight = 768;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Failed to initialise SDL\n");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Demo Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          iGameWidth,
                                          iGameHeight,
                                          SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return -1;
    }
    else
    {
        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("SDL_image could not initialize! SDL_image error: %s\n", IMG_GetError());
            return -1;
        }
    }

    SDL_Texture *texBG = LoadTexture2(renderer, "gfx/background.jpg");
    int iTexBG_w, iTexBG_h;
    SDL_QueryTexture(texBG, NULL, NULL, &iTexBG_w, &iTexBG_h);

    SDL_Texture *texStars = LoadTexture2(renderer, "gfx/background_starsxxx.png");
    int iTexStars_w, iTexStars_h;
    SDL_QueryTexture(texStars, NULL, NULL, &iTexStars_w, &iTexStars_h);

    SDL_Texture *texPlanet = LoadTexture2(renderer, "gfx/poke.png");
    int iTexPlanet_w, iTexPlanet_h;
    SDL_QueryTexture(texPlanet, NULL, NULL, &iTexPlanet_w, &iTexPlanet_h);

    int x = 0;
    int y = 0;
    int vx = 4;
    int vy = 4;

    int xBG = 0;
    int xS = 0;

    // GAME LOOP

    while (true)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                break;
            }
        }

        x += vx;
        y += vy;

        xBG -= 4;
        if (xBG <= -iTexBG_w)
        {
            xBG = 0;
        }

        xS -= 8;
        if (xS <= -iTexStars_w)
        {
            xS = 0;
        }

        if (x + iTexPlanet_w > iGameWidth)
        {
            vx = -vx;
            x = iGameWidth - iTexPlanet_w;
        }
        if (x < 0)
        {
            vx = -vx;
            x = 0;
        }
        if (y + iTexPlanet_h > iGameHeight)
        {
            vy = -vy;
            y = iGameHeight - iTexPlanet_h;
        }
        if (y < 0)
        {
            vy = -vy;
            y = 0;
        }

        // Efface l'écran
        SDL_RenderClear(renderer);

        SDL_Rect rectDestBG1 = {xBG, 0, iTexBG_w, iTexBG_h};
        SDL_RenderCopy(renderer, texBG, NULL, &rectDestBG1);
        SDL_Rect rectDestBG2 = {xBG + iTexBG_w, 0, iTexBG_w, iTexBG_h};
        SDL_RenderCopy(renderer, texBG, NULL, &rectDestBG2);

        SDL_Rect rectDestS1 = {xS, 0, iTexStars_w, iTexStars_h};
        SDL_RenderCopy(renderer, texStars, NULL, &rectDestS1);
        SDL_Rect rectDestS2 = {xS + iTexStars_w, 0, iTexStars_w, iTexStars_h};
        SDL_RenderCopy(renderer, texStars, NULL, &rectDestS2);

        SDL_Rect rectDest = {x, y, iTexPlanet_w, iTexPlanet_h};
        SDL_RenderCopy(renderer, texPlanet, NULL, &rectDest);

        // Présente l'écran
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texPlanet);
    SDL_DestroyTexture(texBG);
    SDL_DestroyTexture(texStars);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
