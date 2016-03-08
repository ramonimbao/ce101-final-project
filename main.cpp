// SDL setup code taken and adapted from lazyfoo.net under SDL2 tutorials

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "tinyfiledialogs.h"
#include <iostream>

// For imgui
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"

#pragma warning(disable:4996) /* allows usage of strncpy, strcpy, st-rcat, sprintf, fopen */

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 120;
const char* inImage = "Original";
const char* outImage = "Anti-aliased";

void loadWindow(std::string path, const char* name, SDL_Window* windowToLoad, SDL_Renderer* rendererToUse, SDL_Surface* surfaceToUse) {
    windowToLoad = SDL_CreateWindow(
                                    name,
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    640,
                                    480,
                                    SDL_WINDOW_SHOWN);
    if (windowToLoad == NULL) printf("Window could not be created! Error: %s\n", SDL_GetError());

    rendererToUse = SDL_CreateRenderer(windowToLoad, -1, SDL_RENDERER_ACCELERATED);
    if (rendererToUse == NULL) printf("Renderer could not be created! Error: %s\n", SDL_GetError());

    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! Error: %s\n", path.c_str(), SDL_GetError());
    } else {
        surfaceToUse = SDL_GetWindowSurface(windowToLoad);
        SDL_BlitSurface(loadedSurface,NULL, surfaceToUse, NULL);
        SDL_UpdateWindowSurface(windowToLoad);

        SDL_FreeSurface(loadedSurface);
    }
}


int main(int argc, char* args[])
{
    // tinyfiledialog picker
    char const * tfdOpen;

    // prepare windows, renderers, surfaces
    SDL_Window* mainWindow = NULL;
    SDL_Window* inWindow = NULL;
    SDL_Window* outWindow = NULL;

    SDL_Renderer* mainRenderer = NULL; //unused?
    SDL_Renderer* inRenderer = NULL;
    SDL_Renderer* outRenderer = NULL;

    SDL_Surface* mainSurface = NULL; //unused?
    SDL_Surface* inSurface = NULL;
    SDL_Surface* outSurface = NULL;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());

    //setup openGL stuff for imgui
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    mainWindow = SDL_CreateWindow(
     "CE101 Project (Bermejo, Corpuz, Imbao)",
     SDL_WINDOWPOS_CENTERED,
     SDL_WINDOWPOS_CENTERED,
     SCREEN_WIDTH,
     SCREEN_HEIGHT,
     SDL_WINDOW_OPENGL
     // SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
     // SDL_WINDOW_SHOWN
    );

    SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);

    //Setup imgui binding
    ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = 460.0f;
        io.DisplaySize.y = 680.0f;
        io.IniFilename = "antialias.ini";
        io.DeltaTime = (1.0f/60.0f);

    ImGui_ImplSdl_Init(mainWindow);

    ImVec4 bgColor = ImColor(114,144,154);
    bool showmainWindow = false;
    // Main Loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdl_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        ImGui_ImplSdl_NewFrame(mainWindow);

        // Main UI
        {
            static float bg_alpha = -0.01f;
            static int i = 0;
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

            ImGui::Begin("Supersampling Anti-aliasing", &showmainWindow, ImVec2(294,112), bg_alpha, window_flags);
            if (ImGui::Button("Load Image")) {
                tfdOpen = tinyfd_openFileDialog("Load Image File", "", 0,NULL, NULL, 0);
                printf("%s\n",tfdOpen);
                loadWindow(tfdOpen, inImage, inWindow, inRenderer, inSurface);
                //TODO: Create image loading function here
            }
            ImGui::Text("Anti-aliasing Factor");
            ImGui::SliderInt("", &i, 0, 10);
            if (ImGui::Button("Apply")) {
                //TODO: Add loading image here
            }
            ImGui::End();
        }

        // Render imgui to window
        glViewport(0,0,(int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(mainWindow);
    }

    // Cleanupgithub
    // close();
    SDL_DestroyRenderer(inRenderer);
    SDL_DestroyRenderer(outRenderer);
    SDL_DestroyWindow(inWindow);
    SDL_DestroyWindow(outWindow);

    inRenderer = NULL;
    outRenderer = NULL;
    inWindow = NULL;
    outWindow = NULL;

    ImGui_ImplSdl_Shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}





#pragma warning(default:4996)

