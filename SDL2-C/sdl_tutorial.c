/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <assert.h>

#include "l_texture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int ALPHA_DELTA = 32;

//Starts up SDL and creates window
int init();

//Loads media
int loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Texture* loadTexture(char *path);

//The window we'll be rendering to
SDL_Window *gWindow = NULL;

//The window renderer
SDL_Renderer *gRenderer = NULL;

//Scene texture
LTexture *gModulatedTexture;

LTexture *gBackgroundTexture;


int init() {
	//Initialization flag
	int success = 1;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = 0;
	} else {
		//Set texture filtering to linear
		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf( "Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow(
                             "SDL Tutorial",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN
                        );

		if(gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = 0;
		} else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n",
                                        SDL_GetError());
				success = 0;
			} else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n",
                                                IMG_GetError()
                                        );
					success = 0;
				}
			}
		}
	}

	return success;
}


int loadMedia()
{
	//Loading success flag
	int success = 1;

	gModulatedTexture = LTexture_create();
	gBackgroundTexture = LTexture_create();

        //Load Foo' texture
	if(!LTexture_loadFromFile(gModulatedTexture, "fadeout.png")) {
                printf( "Failed to load front texture!\n" );
 		success = 0;
	} else {
                LTexture_setBlendMode(gModulatedTexture, SDL_BLENDMODE_BLEND);
	}

	if(!LTexture_loadFromFile(gBackgroundTexture, "fadein.png")){
                printf( "Failed to load background texture!\n" );
 		success = 0;
	}

	return success;
}

void close()
{
	//Free loaded images
	LTexture_free(gModulatedTexture);
	LTexture_free(gBackgroundTexture);

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(char* path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	} else {
		//Create texture from surface pixels
                newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n",
                                path,
                                SDL_GetError()
                        );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int event_loop()
{
        //Main loop flag
        int running = 1;

        //Event handler
        SDL_Event e;

        // Modulation components
        unsigned char alpha = 255;

        //While application is running
        while(running) {
                //Handle events on queue
                while(SDL_PollEvent(&e) != 0) {
                        //User requests quit
                        if(e.type == SDL_QUIT) {
                                running = 0;
                        } else if(e.type == SDL_KEYDOWN) {
                                if(e.key.keysym.sym == SDLK_w){
                                        if(alpha + ALPHA_DELTA > 255) alpha = 255;
                                        else alpha += ALPHA_DELTA;
                                } else if (e.key.keysym.sym == SDLK_s){
                                        if(alpha - ALPHA_DELTA < 0) alpha = 0;
                                        else alpha -= ALPHA_DELTA;
                                }
                                printf("alpha: %d\n", alpha);
                        }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                LTexture_render(gBackgroundTexture, 0, 0, NULL);

                //Modulate and render texture
                LTexture_setAlpha(gModulatedTexture, alpha);
                LTexture_render(gModulatedTexture, 0, 0, NULL);

                //Update screen
                SDL_RenderPresent(gRenderer);
        }

        close();

        return 0;
}


int main(int argc, char* args[])
{
        if(!init()) {
		printf( "Failed to initialize!\n" );
		goto finalize;
	}
		//Load media
        if(!loadMedia()) {
                printf( "Failed to load media!\n" );
                goto finalize;
        }
        event_loop();


	finalize:
	//Free resources and close SDL
                close();
                return 0;
}
