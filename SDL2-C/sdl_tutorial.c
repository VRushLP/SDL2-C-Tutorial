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

//Scene sprites
SDL_Rect gSpriteClips[4];
LTexture *gSpriteSheetTexture;


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

	gSpriteSheetTexture = LTexture_create(gRenderer);

        //Load Foo' texture
	if(!LTexture_loadFromFile(gSpriteSheetTexture, "dots.png")) {
                printf("Failed to load sprite sheet texture!\n");
		success = 0;
	} else {
	        gSpriteClips[0].x = 0;
	        gSpriteClips[0].y = 0;
	        gSpriteClips[0].w = 100;
	        gSpriteClips[0].h = 100;

	        gSpriteClips[1].x = 100;
	        gSpriteClips[1].y = 0;
	        gSpriteClips[1].w = 100;
	        gSpriteClips[1].h = 100;

	        gSpriteClips[2].x = 0;
	        gSpriteClips[2].y = 100;
	        gSpriteClips[2].w = 100;
	        gSpriteClips[2].h = 100;

	        gSpriteClips[3].x = 100;
	        gSpriteClips[3].y = 100;
	        gSpriteClips[3].w = 100;
	        gSpriteClips[3].h = 100;

	}

	return success;
}

void close()
{
	//Free loaded images
	assert(gSpriteSheetTexture != NULL);
	LTexture_free(gSpriteSheetTexture);

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	gSpriteSheetTexture = NULL;

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


int main(int argc, char* args[])
{
        if(!init()) {
		printf( "Failed to initialize!\n" );
	} else {
		//Load media
		if(!loadMedia()) {
			printf( "Failed to load media!\n" );
		} else {
			//Main loop flag
			int quit = 0;

			//Event handler
			SDL_Event e;

			//While application is running
			while(!quit) {
				//Handle events on queue
				while(SDL_PollEvent(&e) != 0) {
					//User requests quit
					if(e.type == SDL_QUIT) {
                                                quit = 1;
					}
				}

                                //Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				LTexture_render(gSpriteSheetTexture, 0, 0, &gSpriteClips[0]);
                                LTexture_render(gSpriteSheetTexture, SCREEN_WIDTH-gSpriteClips[1].w, 0, &gSpriteClips[1]);
                                LTexture_render(gSpriteSheetTexture, 0, SCREEN_HEIGHT-gSpriteClips[2].h, &gSpriteClips[2]);
				LTexture_render(gSpriteSheetTexture, SCREEN_WIDTH-gSpriteClips[3].w, SCREEN_HEIGHT-gSpriteClips[ 3 ].h, &gSpriteClips[ 3 ]);

				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}
	//Free resources and close SDL
	close();

	return 0;
}
