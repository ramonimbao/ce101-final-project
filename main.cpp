// SDL setup code taken and adapted from lazyfoo.net under SDL2 tutorials

#include <SDL2/SDL.h>
#include <stdio.h>
#include "tinyfiledialogs.h"

// For imgui
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"

#pragma warning(disable:4996) /* allows usage of strncpy, strcpy, st-rcat, sprintf, fopen */

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;

int main(int argc, char* args[])
{
	// Setup file opener
	char const * lTheOpenFileName;
	FILE * lIn;

//	lTheOpenFileName = tinyfd_openFileDialog("Get Image File","",0,NULL,NULL,0);

    // Window setup
    SDL_Window* window = NULL;

    SDL_Surface* screenSurface = NULL;

    SDL_Surface* imageToLoad = NULL;

//    if (SDL_Init(SDL_INIT_VIDEO) < 0)
//    {
//        printf(
//            "SDL could not initialize! SDL Error: %s\n",
//            SDL_GetError()
//        );
//    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf(
            "SDL could not initialize! SDL Error: %s\n",
            SDL_GetError()
        );
    }

    //Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    window = SDL_CreateWindow(
        "CE101 Final Project (Bermejo, Corpuz, Imbao)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE
//        SDL_WINDOW_SHOWN
    );

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    // Setup ImGui binding
    ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = 1000.0f;
        io.DisplaySize.y = 700.0f;
        io.IniFilename = "antialias.ini";
        io.DeltaTime = (1.0f/60.0f);

    ImGui_ImplSdl_Init(window);

    bool show_test_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // Main loop
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
        ImGui_ImplSdl_NewFrame(window);

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Begin("Supersampling Anti-aliasing", &show_test_window);
            if (ImGui::Button("Load Image")) {
//              show_test_window ^= 1;
                lTheOpenFileName = tinyfd_openFileDialog("Get Image File","",0,NULL,NULL,0);
            }
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            ImGui::End();
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplSdl_Shutdown();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();

//    if (window == NULL)
//    {
//        printf(
//            "Window could not be created! SDL Error: %s\n",
//            SDL_GetError()
//        );
//    }
//    else
//    {
//        printf(lTheOpenFileName);
//        screenSurface = SDL_GetWindowSurface(window);
//        imageToLoad = SDL_LoadBMP(lTheOpenFileName);
//
//        // Apply image
//        SDL_BlitSurface( imageToLoad, NULL, screenSurface, NULL );
//        SDL_UpdateWindowSurface( window );
//    }
//
//    // Main loop and window event handling
//    bool quit = false;
//
//    SDL_Event e;
//
//    while (!quit)
//    {
//        while (SDL_PollEvent(&e) != 0)
//        {
//            if (e.type == SDL_QUIT)
//            {
//                quit = true;
//            }
//        }
//
//        // Actual main loop
//        SDL_UpdateWindowSurface(window);
//    }
//
//    // Cleanup
//    SDL_DestroyWindow(window);
//    SDL_Quit();

    return 0;
}

#pragma warning(default:4996)

