// SDL setup code taken and adapted from lazyfoo.net under SDL2 tutorials

#include <SDL2/SDL.h>
#include <stdio.h>
#include "tinyfiledialogs.h"

#pragma warning(disable:4996) /* allows usage of strncpy, strcpy, strcat, sprintf, fopen */

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	// Setup file opener
	char const * lTheOpenFileName;
	FILE * lIn;


	lTheOpenFileName = tinyfd_openFileDialog("Get Image File","",0,NULL,NULL,0);
    // Window setup
    SDL_Window* window = NULL;

    SDL_Surface* screenSurface = NULL;

    SDL_Surface* imageToLoad = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf(
            "SDL could not initialize! SDL Error: %s\n",
            SDL_GetError()
        );
    }

    window = SDL_CreateWindow(
        "CE101 Final Project (Bermejo, Corpuz, Imbao)",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL)
    {
        printf(
            "Window could not be created! SDL Error: %s\n",
            SDL_GetError()
        );
    }
    else
    {
        printf(lTheOpenFileName);
        screenSurface = SDL_GetWindowSurface(window);
        imageToLoad = SDL_LoadBMP(lTheOpenFileName);

        // Apply image
        SDL_BlitSurface( imageToLoad, NULL, screenSurface, NULL );
        SDL_UpdateWindowSurface( window );
    }

    // Main loop and window event handling
    bool quit = false;

    SDL_Event e;

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Actual main loop
        SDL_UpdateWindowSurface(window);
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#pragma warning(default:4996)

