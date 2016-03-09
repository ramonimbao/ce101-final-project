// SDL setup code taken and adapted from lazyfoo.net under SDL2 tutorials

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "tinyfiledialogs.h"
#include <iostream>

#include <sstream>

// For imgui
#include <SDL2/SDL_opengl.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"

#pragma warning(disable:4996) /* allows usage of strncpy, strcpy, st-rcat, sprintf, fopen */

//Screen dimension constants
const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 120;

//Total windows
const int TOTAL_WINDOWS = 3;

const char* inImage = "Original";
const char* outImage = "Anti-aliased";

    // prepare windows, renderers, surfaces
SDL_Window* mainWindow = NULL;
SDL_Window* inWindow = NULL;

SDL_Renderer* inRenderer = NULL;

SDL_Surface* inSurface = NULL;
SDL_Surface* outSurface = NULL;

SDL_Texture* inTexture = NULL;
SDL_Texture* outTexture = NULL;

SDL_Rect origPic; //maybe make global?
SDL_Rect aaPic;

SDL_Rect view;


int mainWindowID;
int inWindowID;

int inScreenW;
int inScreenH;

void close()
{
	//Destroy windows
	for( int i = 0; i < TOTAL_WINDOWS; ++i )
	{
//		gWindows[ i ].free();
	}
	//Quit SDL subsystems

    SDL_DestroyRenderer(inRenderer);
    SDL_DestroyWindow(inWindow);

    inRenderer = NULL;
    inWindow = NULL;

//    ImGui_ImplSdl_Shutdown();
//    SDL_GL_DeleteContext(glContext);
//    SDL_DestroyWindow(mainWindow);
//	SDL_Quit();
}


void handleEvent( SDL_Event& e, SDL_Window* mWindow, int mainWindowID)
{
	//If an event was detected for this window
	if( e.type == SDL_WINDOWEVENT && e.window.windowID == mainWindowID )
	{
		switch( e.window.event )
		{
//			//Window appeared
//			case SDL_WINDOWEVENT_SHOWN:
//			mShown = true;
//			break;
//
//			//Window disappeared
//			case SDL_WINDOWEVENT_HIDDEN:
//			mShown = false;
//			break;
//
//			//Get new dimensions and repaint
//			case SDL_WINDOWEVENT_SIZE_CHANGED:
//			mWidth = e.window.data1;
//			mHeight = e.window.data2;
//			SDL_RenderPresent( mRenderer );
//			break;

//			//Repaint on expose
//			case SDL_WINDOWEVENT_EXPOSED:
//			SDL_RenderPresent( mRenderer );
//			break;

			//Hide on close
			case SDL_WINDOWEVENT_CLOSE:
			SDL_DestroyWindow( mWindow );
			break;
		}

	}
}

void render(SDL_Renderer* renderer, SDL_Texture* intexture, SDL_Texture* outtexture){
        //Clear screen
		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 );
		SDL_RenderClear( renderer );
		//Update screen
		SDL_RenderCopy( renderer, intexture, &view, &origPic);
		SDL_RenderCopy( renderer, outtexture, &view, &aaPic);
		SDL_RenderPresent( renderer );
}

void loadWindow(SDL_Surface* insurface, SDL_Surface* outsurface, int surfaceWidth, int surfaceHeight) {
    //Setup original picture and anti-aliased picture
//    SDL_Rect origPic; //maybe make global?
    origPic.w = 500;
    origPic.h = 500;
    origPic.x = 0;
    origPic.y = 0;

//    SDL_Rect aaPic; //maybe make global?
    aaPic.w = 500;
    aaPic.h = 500;
    aaPic.x = 500;
    aaPic.y = 0;

    inWindow = SDL_CreateWindow(
        "Supersampling Anti-aliasing",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1000,
        500,
        SDL_WINDOW_SHOWN);

    inRenderer = SDL_CreateRenderer(inWindow, -1, SDL_RENDERER_ACCELERATED);

    inTexture = SDL_CreateTextureFromSurface(inRenderer, insurface);
    outTexture = SDL_CreateTextureFromSurface(inRenderer, outsurface);

    render(inRenderer, inTexture, outTexture);

}

int main( int argc, char* args[] )
{
    char const * tfdOpen;
    SDL_GLContext glContext;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
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
           );

        mainWindowID = SDL_GetWindowID( mainWindow );

        glContext = SDL_GL_CreateContext(mainWindow);

        //Setup imgui binding
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = 300.0f;
        io.DisplaySize.y = 120.0f;
        io.IniFilename = "antialias.ini";
        io.DeltaTime = (1.0f/60.0f);

        ImGui_ImplSdl_Init(mainWindow);

        ImVec4 bgColor = ImColor(114,144,154);
        bool showmainWindow = true;

        // Set up image viewport stuff
        view.w = 500;
        view.h = 500;
        view.x = 0;
        view.y = 0;

		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;
        bool dragging = false;

		//While application is running
		while( !quit )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ))
			{
			    handleEvent(e, mainWindow, mainWindowID);
			    handleEvent(e, inWindow, inWindowID);

			    ImGui_ImplSdl_ProcessEvent(&e);
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}

				if ( e.type == SDL_MOUSEBUTTONDOWN && e.window.windowID == inWindowID )
                {
                    dragging = true;
                }

                if ( e.type == SDL_MOUSEBUTTONUP && e.window.windowID == inWindowID )
                {
                    dragging = false;
                }

                if ( dragging )
                {
                    if (e.motion.xrel < 250 || e.motion.yrel < 250)
                    {
                        view.x -= e.motion.xrel;
                        view.y -= e.motion.yrel;
                        if (view.x < 0) view.x = 0;
                        if (view.x > inSurface->w - 500) view.x = inSurface->w - 500;
                        if (view.y < 0) view.y = 0;
                        if (view.y > inSurface->h - 500) view.y = inSurface->h - 500;

                    }
                    render(inRenderer, inTexture, outTexture);
                }
			}






        ImGui_ImplSdl_NewFrame(mainWindow);

        // Main UI

            static float bg_alpha = -0.01f;
            static int i = 0;
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

            ImGui::Begin("Supersampling Anti-aliasing", &showmainWindow, ImVec2(294,112), bg_alpha, window_flags);
            if (ImGui::Button("Load Image")) {
            // THIS IS WHERE THE LOAD IMAGE BUTTON HAPPENS
                tfdOpen = tinyfd_openFileDialog("Load Image File", "", 0,NULL, NULL, 0);
                printf("%s\n",tfdOpen);

                inSurface = SDL_LoadBMP(tfdOpen);
                if (inSurface == NULL) {
                        printf("Unable to load image %s! Error: %s\n", tfdOpen, SDL_GetError());
                } else {
                    inScreenW = inSurface->w;
                    inScreenH = inSurface->h;

                    loadWindow(inSurface, outSurface, inScreenW, inScreenH);
                    inWindowID = SDL_GetWindowID( inWindow );
                                 // Add loading function here
                }
            }
            ImGui::Text("Anti-aliasing Factor");
            ImGui::SliderInt("", &i, 0, 10);
            if (ImGui::Button("Apply")) {
            // THIS IS WHERE THE APPLY BUTTON HAPPENS
                //TODO: Add loading image here
                // POSSIBILITY OF ERROR
                outSurface = SDL_LoadBMP(tfdOpen);
                /*SDL_DestroyWindow(inWindow);
                inWindow = NULL;
                loadWindow(inSurface, outSurface, inScreenW, inScreenH);
                inWindowID = SDL_GetWindowID( inWindow );*/
                outTexture = SDL_CreateTextureFromSurface(inRenderer, outSurface);
                render(inRenderer, inTexture, outTexture);

            }
            ImGui::End();


        // Render imgui to window
        glViewport(0,0,(int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(mainWindow);
			}
		}



	//Free resources and close SDL
	close();
	ImGui_ImplSdl_Shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(mainWindow);
	SDL_Quit();

	return 0;
}

#pragma warning(default:4996)

