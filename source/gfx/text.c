#include "text.h"
#include "image.h"

GFX_Font GFX_LoadFont(const char* path, int charWidth, int charHeight) {
	GFX_Font ret;
	GFX_LoadImage(&ret.canvas, path);
	ret.charWidth  = charWidth;
	ret.charHeight = charHeight;

	assert(ret.canvas.width % ret.charWidth == 0);
	assert(ret.canvas.height % ret.charHeight == 0);

	for (int x = 0; x < ret.canvas.width; ++ x) {
		for (int y = 0; y < ret.canvas.height; ++ y) {
			if (GFX_GetPixel(&ret.canvas, x, y) == GFX_ColourToPixel(0, 0, 0, 255)) {
				GFX_SetPixel(&ret.canvas, x, y, GFX_ColourToPixel(0, 0, 0, 0));
			}
		}
	}

	return ret;
}

void GFX_DrawText(GFX_Font* font, GFX_Canvas* canvas, const char* text, int x, int y) {
	for (int i = 0; i < (int) strlen(text); ++ i) {
		GFX_Rect src;
		src.x = ((int) text[i]) % (font->canvas.width / font->charWidth) * font->charWidth;
		src.y = ((int) text[i]) / (font->canvas.width / font->charWidth) * font->charHeight;
		src.w = font->charWidth;
		src.h = font->charHeight;

		GFX_Rect dest;
		dest.x = x + (i * font->charWidth);
		dest.y = y;
		dest.w = font->charWidth;
		dest.h = font->charHeight;

		GFX_BlitCanvas(canvas, &font->canvas, &dest, &src);
	}
}
