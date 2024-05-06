#include <stdio.h>
#include "image.h"

static uint32_t GetSurfaceColour(SDL_Surface* surface, int x, int y) {
	int    bpp  = surface->format->BytesPerPixel;
	Uint8* p    = (uint8_t*) surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
		case 1: return *p;
		case 2: return *(uint16_t*) p;
		case 3: {
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				return p[0] << 16 | p[1] << 8 | p[2];
			}
			else {
				return p[0] | p[1] << 8 | p[2] << 16;
			}
			
			break;
		}
		case 4: {
			return *(Uint32 *)p;
			break;
		}
		default: assert(0);
	}
}

void GFX_LoadImage(GFX_Canvas* canvas, const char* path) {
	SDL_Surface* surface = SDL_LoadBMP(path);

	if (surface == NULL) {
		fprintf(stderr, "Error loading '%s': %s", path, SDL_GetError());
		exit(1);
	}

	GFX_NewCanvas(canvas, surface->w, surface->h);

	for (int x = 0; x < surface->w; ++ x) {
		for (int y = 0; y < surface->h; ++ y) {
			SDL_Color colour;

			SDL_GetRGB(
				GetSurfaceColour(surface, x, y), surface->format, &colour.r, &colour.g,
				&colour.b
			);
			GFX_DrawPixel(
				canvas, x, y, GFX_ColourToPixel(colour.r, colour.g, colour.b, 255)
			);
		}
	}

	SDL_FreeSurface(surface);
}
