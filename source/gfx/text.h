#ifndef BK_GFX_TEXT_H
#define BK_GFX_TEXT_H

#include "canvas.h"
#include "../types.h"

typedef struct {
	GFX_Canvas canvas;
	int        charWidth;
	int        charHeight;
} GFX_Font;

GFX_Font GFX_LoadFont(const char* path, int charWidth, int charHeight);
void     GFX_DrawText(GFX_Font* font, GFX_Canvas* canvas, const char* text, int x, int y);
Vec2     GFX_TextSize(GFX_Font* font, const char* text);

#endif
