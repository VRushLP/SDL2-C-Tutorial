
#include <SDL.h>

#ifndef __L_TEXTURE__
#define __L_TEXTURE__

typedef struct LTexture {
        SDL_Texture* mTexture;
        SDL_Renderer *gRenderer;
        int mWidth;
        int mHeight;
} LTexture;

LTexture* LTexture_create();
void LTexture_destroy(LTexture *texture);
int LTexture_loadFromFile(LTexture *texture, char *path);
void LTexture_free(LTexture *texture);
void LTexture_render(LTexture *texture, int x, int y);

#endif // __L_TEXTURE__
