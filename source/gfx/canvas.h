#ifndef BK_GFX_CANVAS
#define BK_GFX_CANVAS

#include "../common.h"

typedef uint32_t GFX_Colour;

typedef struct {
	int         width;
	int         height;
	GFX_Colour* pixels;
} GFX_Canvas;

typedef struct {
	int x, y;
} GFX_Point;

typedef struct {
	int x, y, w, h;
} GFX_Rect;

void        GFX_NewCanvas(GFX_Canvas* canvas, int width, int height);
void        GFX_FreeCanvas(GFX_Canvas* canvas);
GFX_Colour  GFX_RGBToColour(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void        GFX_DrawPixel(GFX_Canvas* canvas, int x, int y, GFX_Colour col);
void        GFX_ClearCanvas(GFX_Canvas* canvas, uint8_t r, uint8_t g, uint8_t b);
GFX_Colour  GFX_GetPixel(GFX_Canvas* canvas, int x, int y);
void        GFX_CropCanvas(GFX_Canvas* dest, GFX_Canvas* src, int x, int y, int w, int h);
void        GFX_BlitCanvas(
	GFX_Canvas* canvas, GFX_Canvas* from, GFX_Rect* pdest, GFX_Rect* psrc
);
void GFX_HLine(GFX_Canvas* canvas, int x, int y, int length, GFX_Colour colour);
void GFX_VLine(GFX_Canvas* canvas, int x, int y, int length, GFX_Colour colour);

#endif
