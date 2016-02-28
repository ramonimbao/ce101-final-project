// SDL setup code taken and adapted from lazyfoo.net under SDL2 tutorials

#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
    // Window setup
    SDL_Window* window = NULL;

    SDL_Surface* screenSurface = NULL;

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
        screenSurface = SDL_GetWindowSurface(window);
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


