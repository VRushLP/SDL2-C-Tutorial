/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//Starts up SDL and creates window
int init();

//Loads media
int loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface(char *path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;

int init()
{
	//Initialization flag
	int success = 1;

	//Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = 0;
	} else {
		//Create window
		gWindow = SDL_CreateWindow(
                             "SDL Tutorial",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN
                             );
		if(gWindow == NULL ) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = 0;
		} else {
			//Initialize PNG loading
			int imgFlags = IMG_INIT_PNG;
			if(!( IMG_Init(imgFlags) & imgFlags)) {
				printf("SDL_image could not initialize! SDL_image Error: %s\n",
                                        IMG_GetError());
				success = 0;
			} else {
				//Get window surface
				gScreenSurface = SDL_GetWindowSurface( gWindow );
			}
		}
	}

	return success;
}

int loadMedia()
{
	//Loading success flag
	int success = 1;

	gPNGSurface = loadSurface("loaded.png");
	if( gPNGSurface == NULL ) {
		printf("Failed to load stretching image!\n");
		success = 0;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_FreeSurface(gPNGSurface);
	gPNGSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface(char* path)
{
        //The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	printf("Loading %s.\n", path);

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if(loadedSurface == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
	} else {
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if( optimizedSurface == NULL ) {
			printf("Unable to optimize image %s! SDL Error: %s\n",
                                path,
                                SDL_GetError()
                        );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
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

				//Apply the PNG image
				SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL );

				//Update the surface
				SDL_UpdateWindowSurface( gWindow );
			}
		}
	}
	//Free resources and close SDL
	close();

	return 0;
}
