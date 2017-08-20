#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include <assert.h>

#include "l_texture.h"

LTexture* LTexture_create(SDL_Renderer *gRenderer)
{
        LTexture* texture = (LTexture*) (malloc(sizeof(LTexture)));
        texture->mTexture = NULL;
        texture->gRenderer = gRenderer;
        texture->mHeight = 0;
        texture->mWidth = 0;
        return texture;
}

void LTexture_destroy(LTexture *texture)
{
        LTexture_destroy(texture);
        free(texture);
}

int LTexture_loadFromFile(LTexture *texture, char *path)
{
        assert(texture != NULL);
        //Get rid of preexisting texture
	LTexture_free(texture);

	//The final texture
	SDL_Texture *newTexture = NULL;

	//Load image at specified path
	SDL_Surface *loadedSurface = IMG_Load(path);
	if( loadedSurface == NULL ) {
		printf("Unable to load image %s! SDL_image Error: %s\n",
                        path,
                        IMG_GetError());
	} else {
		//Color key image
		SDL_SetColorKey(loadedSurface,
                                SDL_TRUE,
                                SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF ));

		//Create texture from surface pixels
                newTexture = SDL_CreateTextureFromSurface(texture->gRenderer, loadedSurface);
		if( newTexture == NULL ) {
			printf("Unable to create texture from %s! SDL Error: %s\n",
                                path,
                                SDL_GetError());
		} else {
			//Get image dimensions
			texture->mWidth = loadedSurface->w;
			texture->mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
        texture->mTexture = newTexture;
	return texture->mTexture != NULL;
}

void LTexture_free(LTexture *texture)
{
        assert(texture != NULL);
        //Free texture if it exists
	if(texture->mTexture != NULL )
	{
		SDL_DestroyTexture(texture->mTexture );
		texture->mTexture = NULL;
		texture->mWidth = 0;
		texture->mHeight = 0;
	}
}

void LTexture_render(LTexture *texture, int x, int y, SDL_Rect* clip)
{
        assert(texture != NULL);
        // Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, texture->mWidth, texture->mHeight};
	if(clip != NULL){
               renderQuad.w = clip->w;
               renderQuad.h = clip->h;
	}

	// Render to screen
	SDL_RenderCopy(texture->gRenderer, texture->mTexture, clip, &renderQuad);
}

void LTexture_setColor(LTexture *texture, unsigned char red, unsigned char green, unsigned char blue)
{
        assert(texture != NULL);
        //Modulate texture
        SDL_SetTextureColorMod(texture->mTexture, red, green, blue);
}
