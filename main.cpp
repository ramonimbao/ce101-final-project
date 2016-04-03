// SDL setup code taken and adapted from lazyfoo.net under SDL2 tutorials

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include "tinyfiledialogs.h"
#include <iostream>

#include <sstream>
#include <vector>
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
SDL_Surface* bgSurface = NULL;

SDL_Texture* inTexture = NULL;
SDL_Texture* outTexture = NULL;
SDL_Texture* bgTexture = NULL;

SDL_Rect origPic; //maybe make global?
SDL_Rect aaPic;

SDL_Rect view;

std::vector<std::vector<Uint32> > colorsOriginal; //array containing pixel values of original image
std::vector<std::vector<Uint32> > colorsScaledImage; //array containing pixel values of scaled image
std::vector<std::vector<Uint32> > colorsScaledSampledImage; // array containing pixel values of anti-aliased image
std::vector<std::vector<Uint32> > colorsFinalImage; //array containing pixel values of final original-sized image

int mainWindowID;
int inWindowID;

int inScreenW;
int inScreenH;

bool quit;

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
		SDL_RenderCopy( renderer, bgTexture, NULL, NULL );
		SDL_RenderCopy( renderer, intexture, &view, &origPic);
		SDL_RenderCopy( renderer, outtexture, &view, &aaPic);
		SDL_RenderPresent( renderer );
}

void loadWindow(SDL_Surface* insurface, SDL_Surface* outsurface, int surfaceWidth, int surfaceHeight) {
    //Setup original picture and anti-aliased picture
//    SDL_Rect origPic; //maybe make global?
    if (surfaceWidth >= 500) origPic.w = 500;
    else origPic.w = surfaceWidth;
    if (surfaceHeight >= 500) origPic.h = 500;
    else origPic.h = surfaceHeight;
    origPic.x = 50;
    origPic.y = 50;

//    SDL_Rect aaPic; //maybe make global?
    aaPic.w = origPic.w;
    aaPic.h = origPic.h;
    aaPic.x = 600;
    aaPic.y = 50;

    inWindow = SDL_CreateWindow(
        "Supersampling Anti-aliasing",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1150,
        600,
        SDL_WINDOW_SHOWN);

    inRenderer = SDL_CreateRenderer(inWindow, -1, SDL_RENDERER_ACCELERATED);

    inTexture = SDL_CreateTextureFromSurface(inRenderer, insurface);
    outTexture = SDL_CreateTextureFromSurface(inRenderer, outsurface);
    bgTexture = SDL_CreateTextureFromSurface(inRenderer, bgSurface);

    render(inRenderer, inTexture, outTexture);

}

void getRGBValues(SDL_PixelFormat* fmt, Uint32 pixel, Uint8* red, Uint8* green, Uint8* blue) {
    Uint32 temp;
    temp = pixel & fmt->Rmask;
    temp = temp >> fmt->Rshift;
    temp = temp << fmt->Rloss;
    *red = (Uint8) temp;

    temp = pixel & fmt->Gmask;
    temp = temp >> fmt->Gshift;
    temp = temp << fmt->Gloss;
    *green = (Uint8) temp;

    temp = pixel & fmt->Bmask;
    temp = temp >> fmt->Bshift;
    temp = temp << fmt->Bloss;
    *blue = (Uint8) temp;
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
		quit = false;

		//Event handler
		SDL_Event e;
        bool dragging = false;

        // Background loading
        bgSurface = SDL_LoadBMP("bg.bmp");


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
                    if (e.motion.xrel < 50 || e.motion.yrel < 50)
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
            static int factor = 2;
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
                    inSurface = SDL_ConvertSurfaceFormat(inSurface, SDL_PIXELFORMAT_ABGR8888, NULL); // convert to 32 bits
                    inScreenW = inSurface->w;
                    inScreenH = inSurface->h;

                    SDL_DestroyWindow(inWindow);
                    loadWindow(inSurface, outSurface, inScreenW, inScreenH);
                    inWindowID = SDL_GetWindowID( inWindow );

                                 // Add loading function here
                    colorsOriginal.clear();
                    colorsOriginal = std::vector<std::vector<Uint32> >( inScreenH, std::vector<Uint32>(inScreenW, 0) );

                    SDL_PixelFormat *fmt;
                    Uint32 temp, pixel;
                    Uint8 red, green, blue;

                    fmt = inSurface->format;

                    int bpp = inSurface->format->BytesPerPixel;

                    SDL_LockSurface(inSurface);
                    for(int y=0; y<inScreenH; y++) {
                        for (int x=0; x<inScreenW; x++) {

                            pixel = *((Uint32*) (inSurface->pixels + y*inSurface->pitch + x *bpp));

                            colorsOriginal[y][x] = pixel;
                            /*
                            // Grab pixel colors
                            temp = pixel & fmt->Rmask;
                            temp = temp >> fmt->Rshift;
                            temp = temp << fmt->Rloss;
                            red = (Uint8) temp;

                            temp = pixel & fmt->Gmask;
                            temp = temp >> fmt->Gshift;
                            temp = temp << fmt->Gloss;
                            green = (Uint8) temp;

                            temp = pixel & fmt->Bmask;
                            temp = temp >> fmt->Bshift;
                            temp = temp << fmt->Bloss;
                            blue = (Uint8) temp;

                            //printf("(%d, %d, %d)\n",red,green,blue);
                            */
                        }
                    }
                    SDL_UnlockSurface(inSurface);

                    render(inRenderer, inTexture, outTexture);
                }
            }
            ImGui::Text("Anti-aliasing Factor");
            ImGui::SliderInt("", &factor, 2, 8);
            if (ImGui::Button("Apply")) {
            // THIS IS WHERE THE APPLY BUTTON HAPPENS
                //TODO: Add loading image here
                // POSSIBILITY OF ERROR

                //outSurface = SDL_CreateRGBSurfaceFrom(inSurface->pixels, inSurface->w, inSurface->h, 32, inSurface->pitch, inSurface->format->Rmask, inSurface->format->Gmask, inSurface->format->Bmask, inSurface->format->Amask);
                Uint32 rmask, gmask, bmask, amask;
                #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                rmask = 0xff000000;
                gmask = 0x00ff0000;
                bmask = 0x0000ff00;
                amask = 0x000000ff;
                #else
                rmask = 0x000000ff;
                gmask = 0x0000ff00;
                bmask = 0x00ff0000;
                amask = 0xff000000;
                #endif
                outSurface = SDL_CreateRGBSurface(NULL, inSurface->w, inSurface->h, 32, rmask, gmask, bmask, amask);

                std::cout << "Starting...";
                    SDL_PixelFormat *fmt;
                    Uint32 temp, pixel;
                    Uint8 red, green, blue;
                    int outScreenW = outSurface->w;
                    int outScreenH = outSurface->h;
                    fmt = outSurface->format;
                    int bpp = outSurface->format->BytesPerPixel;

                    std::cout << "\nScaling...";
                    colorsScaledImage.clear();
                    colorsScaledImage = std::vector<std::vector<Uint32> >( inScreenH*factor, std::vector<Uint32>(inScreenW*factor, 0) );

                    // copy all the colors to the bigger array, making copies
                    for (unsigned int y=0; y<inScreenH*factor; y++){
                        for (unsigned int x=0; x<inScreenW*factor; x++){
                            colorsScaledImage[y][x] = colorsOriginal[y/factor][x/factor];
                        }
                    }

                    std::cout << "\nSampling...";
                    colorsScaledSampledImage.clear();
                    colorsScaledSampledImage = std::vector<std::vector<Uint32> >( inScreenH*factor, std::vector<Uint32>(inScreenW*factor, 0) );
                    for (unsigned int y=0; y < inScreenH * factor; y++) {
                        for (unsigned int x=0; x<inScreenW*factor; x++) {
                            // TOP-LEFT CORNER
                            if (x == 0 && y == 0) {
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for (int _y=0; _y<=1; _y++) {
                                    for (int _x=0; _x<=1; _x++) {
                                        getRGBValues(fmt, colorsScaledImage[_y][_x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }

                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/4, gSum/4, bSum/4);
                            }
                            // TOP EDGE (NOT CORNERS)
                            else if (y == 0 && (x >= 1 && x <= inScreenW*factor - 2)) {
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for (int _y=0; _y<=1; _y++) {
                                    for (int _x=-1; _x<=1; _x++) {
                                        getRGBValues(fmt, colorsScaledImage[_y][_x+x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }

                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/6, gSum/6, bSum/6);
                            }

                            // TOP RIGHT CORNER
                            else if(y == 0 && x == inScreenW*factor - 1){
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for (int _y=0; _y<=1; _y++){
                                    for (int _x=-1; _x<=0; _x++){
                                        getRGBValues(fmt, colorsScaledImage[_y][_x+x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }
                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/4, gSum/4, bSum/4);
                            }
                            //LEFT MOST EDGE
                            else if (x == 0 && ( y >= 1 && y <= inScreenH*factor - 2)){
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for (int _y=-1; _y<=1; _y++){
                                    for (int _x=0; _x<=1; _x++){
                                       getRGBValues(fmt, colorsScaledImage[_y+y][_x+x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }
                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/6, gSum/6, bSum/6);
                            }
                            //BOTTOM LEFT CORNER
                            else if(x == 0 && y == inScreenH*factor - 1){
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for (int _y=-1; _y<=0; _y++){
                                    for (int _x=0; _x<=1; _x++){
                                        getRGBValues(fmt, colorsScaledImage[_y+y][_x+x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }
                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/4, gSum/4, bSum/4);
                            }
                            //BOTTOM MOST EDGES
                            else if(y == inScreenH * factor -1 && (x >= 1 && x <= inScreenW*factor - 2)){
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for (int _y=-1; _y<=0; _y++){
                                    for (int _x=-1; _x<=1; _x++){
                                        getRGBValues(fmt, colorsScaledImage[_y+y][_x+x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }
                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/6, gSum/6, bSum/6);
                            }
                            //BOTTOM RIGHT CORNER
                            else if(y == inScreenH * factor -1 && x == inScreenW * factor -1){
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for (int _y=-1; _y<=0; _y++){
                                    for (int _x=-1; _x<=0; _x++){
                                        getRGBValues(fmt, colorsScaledImage[_y+y][_x+x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }
                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/4, gSum/4, bSum/4);
                            }
                            //RIGHT MOST EDGE
                            else if(x == inScreenW * factor -1 && (y >= 1 && y <= inScreenH *factor -2)){
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for(int _y=-1; _y<=1; _y++){
                                    for (int _x=-1; _x<=0; _x++){
                                        getRGBValues(fmt, colorsScaledImage[_y+y][_x+x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }
                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/6, gSum/6, bSum/6);
                            }
                            else{
                                Uint32 rSum = 0;
                                Uint32 gSum = 0;
                                Uint32 bSum = 0;
                                Uint8 r, g, b;
                                for (int _y=-1; _y<=1; _y++){
                                    for (int _x=-1; _x<=1; _x++){
                                        getRGBValues(fmt, colorsScaledImage[_y+y][_x+x], &r, &g, &b);
                                        rSum += r;
                                        gSum += g;
                                        bSum += b;
                                    }
                                }
                                colorsScaledSampledImage[y][x] = SDL_MapRGB(fmt, rSum/9, gSum/9, bSum/9);
                            }
                        }
                    }

                    std::cout << "\nDownsampling...";
                    colorsFinalImage.clear();
                    colorsFinalImage = std::vector<std::vector<Uint32> >( inScreenH, std::vector<Uint32>(inScreenW, 0) );
                    for (unsigned int y=0; y<inScreenH; y++) {
                        for (unsigned int x=0; x<inScreenW; x++) {
                            Uint32 rSum = 0;
                            Uint32 gSum = 0;
                            Uint32 bSum = 0;
                            Uint8 r, g, b;
                            for(int _y=0; _y<factor; _y++) {
                                for(int _x=0; _x<factor; _x++) {
                                    getRGBValues(fmt, colorsScaledSampledImage[_y+y*factor][_x+x*factor], &r, &g, &b);
                                    rSum += r;
                                    gSum += g;
                                    bSum += b;
                                }
                            }
                            int factor2 = factor * factor;
                            colorsFinalImage[y][x] = SDL_MapRGB(fmt, rSum/factor2, gSum/factor2, bSum/factor2);
                        }
                    }

                    SDL_LockSurface(outSurface);
                    for(unsigned int y=0; y<inScreenH; y++) {
                        for (unsigned int x=0; x<inScreenW; x++) {
                            pixel = colorsFinalImage[y][x];

                            Uint32* curPixel = (Uint32*)outSurface->pixels;
                            Uint32* p = curPixel + y*outSurface->pitch/4 + x;
                            *p = pixel;
                        }
                    }
                    SDL_UnlockSurface(outSurface);



                    std::cout << " ...done!\n";

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


    //
	//Free resources and close SDL
	close();
	ImGui_ImplSdl_Shutdown();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(mainWindow);
	SDL_Quit();

	return 0;
}

#pragma warning(default:4996)

