#ifndef BK_GFX_SCREEN_H
#define BK_GFX_SCREEN_H

#include <SDL2/SDL.h>
#include "canvas.h"
#include "../common.h"

typedef struct {
	SDL_Window*   window;
	SDL_Renderer* renderer;
	SDL_Texture*  texture;
} GFX_Screen;

void GFX_InitScreen(GFX_Screen* screen, size_t width, size_t height);
void GFX_FreeScreen(GFX_Screen* screen);
void GFX_RenderScreen(GFX_Screen* screen, GFX_Canvas* canvas);

#endif
