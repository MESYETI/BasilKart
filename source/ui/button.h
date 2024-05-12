#ifndef BK_UI_BUTTON_H
#define BK_UI_BUTTON_H

#include "../ui.h"
#include "../gfx/text.h"

typedef enum {
	UI_BUTTON_STYLE_ARROWS = 0
} UI_ButtonStyle;

typedef void (*UI_ButtonAction)(UI_TableEntry* button);

typedef struct {
	UI_ButtonStyle  style;
	const char*     label;
	UI_ButtonAction onClick;
	GFX_Font*       font;
} UI_Button;

UI_TableEntry UI_NewButton(
	UI_ButtonStyle style, GFX_Font* font, const char* label, UI_ButtonAction onClick
);

#endif
