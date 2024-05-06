#ifndef BK_GFX_CANVAS
#define BK_GFX_CANVAS

#include "../common.h"

typedef uint32_t GFX_Pixel;

typedef struct {
	uint8_t r, g, b, a;
} GFX_Colour;

typedef struct {
	int         width;
	int         height;
	GFX_Pixel* pixels;
} GFX_Canvas;

typedef struct {
	int x, y;
} GFX_Point;

typedef struct {
	int x, y, w, h;
} GFX_Rect;

void        GFX_NewCanvas(GFX_Canvas* canvas, int width, int height);
void        GFX_FreeCanvas(GFX_Canvas* canvas);
GFX_Pixel   GFX_ColourToPixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
GFX_Colour  GFX_PixelToColour(GFX_Pixel colour);
void        GFX_DrawPixel(GFX_Canvas* canvas, int x, int y, GFX_Pixel col);
void        GFX_ClearCanvas(GFX_Canvas* canvas, uint8_t r, uint8_t g, uint8_t b);
GFX_Pixel   GFX_GetPixel(GFX_Canvas* canvas, int x, int y);
GFX_Pixel   GFX_LerpPixel(GFX_Pixel a, GFX_Pixel b, double t);
void        GFX_CropCanvas(GFX_Canvas* dest, GFX_Canvas* src, int x, int y, int w, int h);
void        GFX_BlitCanvas(
	GFX_Canvas* canvas, GFX_Canvas* from, GFX_Rect* pdest, GFX_Rect* psrc
);
void GFX_HLine(GFX_Canvas* canvas, int x, int y, int length, GFX_Pixel colour);
void GFX_VLine(GFX_Canvas* canvas, int x, int y, int length, GFX_Pixel colour);

#endif
