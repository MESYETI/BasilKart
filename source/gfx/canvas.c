#include <math.h>
#include <stdio.h>
#include "canvas.h"
#include "../util.h"
#include "../safe.h"
#include "../types.h"

void GFX_NewCanvas(GFX_Canvas* canvas, int width, int height) {
	canvas->width  = width;
	canvas->height = height;
	canvas->pixels = SafeMalloc(width * height * sizeof(GFX_Pixel));
}

void GFX_FreeCanvas(GFX_Canvas* canvas) {
	free(canvas->pixels);
}

GFX_Pixel GFX_ColourToPixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return r | (g << 8) | (b << 16) | (a << 24);
	//return r + (rand() % 20) | (g + (rand() % 20) << 8) | (b + (rand() % 20) << 16) | (a << 24);
}

GFX_Colour GFX_PixelToColour(GFX_Pixel colour) {
	GFX_Colour ret;
	ret.r =  colour & 0x000000FF;
	ret.g = (colour & 0x0000FF00) >> 8;
	ret.b = (colour & 0x00FF0000) >> 16;
	ret.a = (colour & 0xFF000000) >> 24;
	return ret;
}

void GFX_DrawPixel(GFX_Canvas* canvas, int x, int y, GFX_Pixel col) {
	if ((x >= canvas->width) || (y >= canvas->height) || (x < 0) || (y < 0)) {
		return;
	}

	GFX_Pixel newCol;
	uint8_t   alpha = (col & 0xFF000000) >> 24;

	if (alpha == 0) return;
	if (alpha == 255) {
		newCol = col;
	}
	else {
		GFX_Pixel old = GFX_GetPixel(canvas, x, y);
		newCol        = GFX_LerpPixel(old, col, (double) alpha / 255.0);
	}

	//canvas->pixels[(y * canvas->width) + x] = newCol;
	GFX_SetPixel(canvas, x, y, newCol);
}

void GFX_SetPixel(GFX_Canvas* canvas, int x, int y, GFX_Pixel col) {
	if ((x >= canvas->width) || (y >= canvas->height) || (x < 0) || (y < 0)) {
		return;
	}

	canvas->pixels[(y * canvas->width) + x] = col;
}

void GFX_ClearCanvas(GFX_Canvas* canvas, uint8_t r, uint8_t g, uint8_t b) {
	uint32_t colour = GFX_ColourToPixel(r, g, b, 255);

	for (int i = 0; i < canvas->width * canvas->height; ++ i) {
		canvas->pixels[i] = colour;
	}
}

GFX_Pixel GFX_GetPixel(GFX_Canvas* canvas, int x, int y) {
	if ((x < 0) || (y < 0) || (x >= canvas->width) || (y >= canvas->height)) {
		printf("Out of bounds: %d %d for %d %d\n", x, y, canvas->width, canvas->height);
		assert(0);
	}

	return canvas->pixels[(y * canvas->width) + x];
}

GFX_Pixel GFX_LerpPixel(GFX_Pixel a, GFX_Pixel b, double t) {
	GFX_Colour c1 = GFX_PixelToColour(a);
	GFX_Colour c2 = GFX_PixelToColour(b);

	return GFX_ColourToPixel(
		Lerp(c1.r, c2.r, t), Lerp(c1.g, c2.g, t), Lerp(c1.b, c2.b, t),
		Lerp(c1.a, c2.a, t)
	);
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

			GFX_Pixel pixel = GFX_GetPixel(from, srcPos.x, srcPos.y);
			if ((pixel & 0xFF000000) >> 24 == 0) {
				continue;
			}

			GFX_DrawPixel(canvas, pos.x, pos.y, pixel);
		}
	}
}

void GFX_HLine(GFX_Canvas* canvas, int x, int y, int length, GFX_Pixel colour) {
	for (int i = 0; i < length; ++ i) {
		GFX_DrawPixel(canvas, x + i, y, colour);
	}
}

void GFX_VLine(GFX_Canvas* canvas, int x, int y, int length, GFX_Pixel colour) {
	for (int i = 0; i < length; ++ i) {
		GFX_DrawPixel(canvas, x, y + i, colour);
	}
}

void GFX_Line(GFX_Canvas* canvas, Vec2 start, Vec2 end, GFX_Pixel col) {
	int length = GetDistance(start, end);

	start.x = MAX(start.x, 0);
	start.y = MAX(start.y, 0);
	end.x   = MIN(end.x,   canvas->width);
	end.y   = MIN(end.y,   canvas->height);

	for (int i = 0; i < length; ++ i) {
		double t      = i == 0? 0.0 : (double) i / (double) length;
		FVec2  lerped = LerpVec2(start, end, t);

		GFX_DrawPixel(canvas, round(lerped.x), round(lerped.y), col);
	}
}

void GFX_Triangle(GFX_Canvas* canvas, Vec2 p1, Vec2 p2, Vec2 p3, GFX_Pixel col) {
	GFX_Line(canvas, p1, p2, col);
	GFX_Line(canvas, p2, p3, col);
	GFX_Line(canvas, p3, p1, col);
}

void GFX_DrawRect(GFX_Canvas* canvas, GFX_Rect rect, GFX_Pixel colour) {
	GFX_HLine(canvas, rect.x, rect.y, rect.w, colour);
	GFX_VLine(canvas, rect.x, rect.y, rect.h, colour);
	GFX_HLine(canvas, rect.x, rect.y + rect.h - 1, rect.w, colour);
	GFX_VLine(canvas, rect.x + rect.w - 1, rect.y, rect.h, colour);
}
