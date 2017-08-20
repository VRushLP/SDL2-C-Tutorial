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

	gModulatedTexture = LTexture_create(gRenderer);

        //Load Foo' texture
	if(!LTexture_loadFromFile(gModulatedTexture, "colors.png")) {
                printf("Failed to load sprite sheet texture!\n");
		success = 0;
	}

	return success;
}

void close()
{
	//Free loaded images
	LTexture_free(gModulatedTexture);

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
        int quit = 0;

        //Event handler
        SDL_Event e;

        // Modulation components
        unsigned char r = 255;
        unsigned char g = 255;
        unsigned char b = 255;

        //While application is running
        while(!quit) {
                //Handle events on queue
                while(SDL_PollEvent(&e) != 0) {
                        //User requests quit
                        if(e.type == SDL_QUIT) {
                                quit = 1;
                        } else if(e.type == SDL_KEYDOWN) {
                                switch(e.key.keysym.sym) {
                                        //Increase red
                                        case SDLK_q:
                                                r += 32;
                                                break;

                                        //Increase green
                                        case SDLK_w:
                                                g += 32;
                                                break;

                                        //Increase blue
                                        case SDLK_e:
                                                b += 32;
                                                break;

                                        //Decrease red
                                        case SDLK_a:
                                                r -= 32;
                                                break;

                                        //Decrease green
                                        case SDLK_s:
                                                g -= 32;
                                                break;

                                        //Decrease blue
                                        case SDLK_d:
                                                b -= 32;
                                                break;

                                        case SDLK_r:
                                                r = 255;
                                                g = 255;
                                                b = 255;
                                                break;
                                }
                        }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Modulate and render texture
                LTexture_setColor(gModulatedTexture, r, g, b);
                LTexture_render(gModulatedTexture, 0, 0, NULL);

                //Update screen
                SDL_RenderPresent(gRenderer);
        }
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
