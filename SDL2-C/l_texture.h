
#include <SDL.h>

#ifndef __L_TEXTURE__
#define __L_TEXTURE__

typedef struct LTexture {
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
} LTexture;

LTexture* LTexture_create();
void LTexture_destroy(LTexture *texture);
int LTexture_loadFromFile(LTexture *texture, char *path);
void LTexture_free(LTexture *texture);
void LTexture_render(LTexture *texture, int x, int y, SDL_Rect* clip);
void LTexture_setColor(LTexture *texture, unsigned char red, unsigned char green, unsigned char blue);
void LTexture_setBlendMode(LTexture *texture, SDL_BlendMode blending);
void LTexture_setAlpha(LTexture *texture, unsigned char alpha);

#endif // __L_TEXTURE__
