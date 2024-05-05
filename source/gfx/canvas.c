#include <math.h>
#include <stdio.h>
#include "canvas.h"
#include "../safe.h"

void GFX_NewCanvas(GFX_Canvas* canvas, int width, int height) {
	canvas->width  = width;
	canvas->height = height;
	canvas->pixels = SafeMalloc(width * height * sizeof(GFX_Colour));
}

void GFX_FreeCanvas(GFX_Canvas* canvas) {
	free(canvas->pixels);
}

GFX_Colour GFX_RGBToColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return r | (g << 8) | (b << 16) | (a << 24);
}

void GFX_DrawPixel(GFX_Canvas* canvas, int x, int y, GFX_Colour col) {
	if ((x >= canvas->width) || (y >= canvas->height) || (x < 0) || (y < 0)) {
		return;
	}

	canvas->pixels[(y * canvas->width) + x] = col;
}

void GFX_ClearCanvas(GFX_Canvas* canvas, uint8_t r, uint8_t g, uint8_t b) {
	uint32_t colour = GFX_RGBToColour(r, g, b, 255);

	for (int i = 0; i < canvas->width * canvas->height; ++ i) {
		canvas->pixels[i] = colour;
	}
}

GFX_Colour GFX_GetPixel(GFX_Canvas* canvas, int x, int y) {
	if ((x < 0) || (y < 0) || (x >= canvas->width) || (y >= canvas->height)) {
		printf("Out of bounds: %d %d\n", x, y);
		assert(0);
	}

	return canvas->pixels[(y * canvas->width) + x];
}

void GFX_CropCanvas(GFX_Canvas* dest, GFX_Canvas* src, int x, int y, int w, int h) {
	GFX_NewCanvas(dest, w, h);

	for (int ix = x; ix - x < w; ++ ix) {
		for (int iy = y; iy - y < h; ++ iy) {
			GFX_DrawPixel(dest, ix - x, iy - y, GFX_GetPixel(src, ix, iy));
		}
	}
}

void GFX_BlitCanvas(
	GFX_Canvas* canvas, GFX_Canvas* from, GFX_Rect* pdest, GFX_Rect* psrc
) {
	GFX_Rect src;
	if (psrc == NULL) {
		src = (GFX_Rect) {0, 0, from->width, from->height};
	}
	else {
		src = *psrc;
	}

	GFX_Rect dest;
	if (pdest == NULL) {
		dest = (GFX_Rect) {0, 0, canvas->width, canvas->height};
	}
	else {
		dest = *pdest;
	}

	for (int y = 0; y < dest.h; ++ y) {
		GFX_Rect pos;
		pos.y = dest.y + y;
		
		if (pos.y < 0) {
			continue;
		}
		else if (pos.y >= canvas->width) {
			break;
		}

		for (int x = 0; x < dest.w; ++ x) {
			pos.x = dest.x + x;
			
			if (pos.x < 0) {
				continue;
			}
			if (pos.x >= canvas->width) {
				break;
			}

			GFX_Point srcPos = (GFX_Point) {
				floor((double) x / dest.w * src.w + src.x),
				floor((double) y / dest.h * src.h + src.y)
			};

			GFX_Colour pixel = GFX_GetPixel(from, srcPos.x, srcPos.y);
			if ((pixel & 0xFF000000) >> 24 == 0) {
				continue;
			}

			GFX_DrawPixel(canvas, pos.x, pos.y, pixel);
		}
	}
}

void GFX_HLine(GFX_Canvas* canvas, int x, int y, int length, GFX_Colour colour) {
	for (int i = 0; i < length; ++ i) {
		GFX_DrawPixel(x + i, y, colour);
	}
}

void GFX_VLine(GFX_Canvas* canvas, int x, int y, int length, GFX_Colour colour) {
	for (int i = 0; i < length; ++ i) {
		GFX_DrawPixel(x, y + i, colour);
	}
}
