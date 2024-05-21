#include "screen.h"

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

	screen->texture = SDL_CreateTexture(
		screen->renderer, SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING, width, height
	);

	if (screen->texture == NULL) {
		fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_RenderSetLogicalSize(screen->renderer, width, height);
}

void GFX_FreeScreen(GFX_Screen* screen) {
	SDL_DestroyTexture(screen->texture);
	SDL_DestroyRenderer(screen->renderer);
	SDL_DestroyWindow(screen->window);
	SDL_Quit();
}

void GFX_RenderScreen(GFX_Screen* screen, GFX_Canvas* canvas) {
	SDL_RenderClear(screen->renderer);
	SDL_UpdateTexture(screen->texture, NULL, canvas->pixels, canvas->width * 4);
	SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
	SDL_RenderPresent(screen->renderer);
}
