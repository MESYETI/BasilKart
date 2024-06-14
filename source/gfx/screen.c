#include <stdio.h>
#include "screen.h"
#include "../util.h"

void GFX_InitScreen(GFX_Screen* screen, size_t width, size_t height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	screen->window = SDL_CreateWindow(
		"Basil Kart", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width * 2, height * 2,SDL_WINDOW_RESIZABLE
	);

	if (screen->window == NULL) {
		fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
		exit(1);
	}

	Log("Created window");

	#ifdef __3DS__
		screen->renderer = SDL_CreateRenderer(
			screen->window, -1, SDL_RENDERER_PRESENTVSYNC
		);
	#else
		screen->renderer = SDL_CreateRenderer(
			screen->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);
	#endif

	if (screen->renderer == NULL) {
		fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

	Log("Created renderer");

	screen->texture = SDL_CreateTexture(
		screen->renderer, SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING, width, height
	);

	if (screen->texture == NULL) {
		fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
		exit(1);
	}

	Log("Created screen texture");

	SDL_RenderSetLogicalSize(screen->renderer, width, height);
}

void GFX_FreeScreen(GFX_Screen* screen) {
	SDL_DestroyTexture(screen->texture);
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyWindow(screen->window);
	SDL_Quit();
}

void GFX_RenderScreen(GFX_Screen* screen, GFX_Canvas* canvas) {
	/*for (int x = 0; x < canvas->width; ++ x) {
		for (int y = 0; y < canvas->height; ++ y) {
			GFX_Colour pixel   = GFX_PixelToColour(GFX_GetPixel(canvas, x, y));
			uint8_t    average = (pixel.r + pixel.g + pixel.b) / 3;

			if (average > 128) {
				GFX_DrawPixel(canvas, x, y, GFX_ColourToPixel(255, 255, 255, 255));
			}
			else {
				GFX_DrawPixel(canvas, x, y, GFX_ColourToPixel(0, 0, 0, 255));
			}
		}
	}*/

	SDL_RenderClear(screen->renderer);
	SDL_UpdateTexture(screen->texture, NULL, canvas->pixels, canvas->width * 4);
	SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
	SDL_RenderPresent(screen->renderer);
}
