#ifndef BK_UI_LABEL_H
#define BK_UI_LABEL_H

#include "../ui.h"
#include "../gfx/text.h"

typedef struct {
	const char* label;
	GFX_Font*   font;
} UI_Label;

UI_TableEntry UI_NewLabel(GFX_Font* font, const char* plabel);

#endif
