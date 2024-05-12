#include "button.h"
#include "../safe.h"

static void Free(UI_TableEntry* this) {
	UNUSED(this);
}

static void HandleEvent(UI_TableEntry* this, SDL_Event* e) {
	UNUSED(this);
}

static void OnAction(UI_TableEntry* this) {
	UNUSED(this);
}

static void Render(GFX_Canvas* canvas, UI_TableEntry* this) {
	UI_Button* button    = (UI_Button*) this->data;
	char*      label     = SafeMalloc(strlen(button->label + 5));
	sprintf(label, "> %s <", button->label);

	Vec2 labelSize = GFX_TextSize(button->font, label);
	Vec2 labelPos  = (Vec2) {
		this->rect.x + (this->rect.w / 2) - (labelSize.x / 2),
		this->rect.y + (this->rect.h / 2) - (labelSize.y / 2)
	};

	GFX_DrawText(button->font, canvas, label, labelPos.x, labelPos.y);
	GFX_DrawRect(canvas, this->rect, 0xFFFFFFFF);
}

UI_TableEntry UI_NewButton(
	UI_ButtonStyle style, GFX_Font* font, const char* label, UI_ButtonAction onClick
) {
	UI_TableEntry ret;
	ret.data        = (void*) NEW(UI_Button);
	ret.free        = &Free;
	ret.handleEvent = &HandleEvent;
	ret.onAction    = &OnAction;
	ret.render      = &Render;

	UI_Button* button = (UI_Button*) ret.data;
	button->style     = style;
	button->label     = label;
	button->onClick   = onClick;
	button->font      = font;
	return ret;
}
