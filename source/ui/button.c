#include "button.h"
#include "../safe.h"
#include "../util.h"

static void Free(UI_TableEntry* this) {
	UNUSED(this);
}

static void HandleEvent(UI_TableEntry* this, SDL_Event* e) {
	UNUSED(this);
	UNUSED(e);
}

static void OnAction(UI_TableEntry* this) {
	UNUSED(this);
}

static char* GetLabel(UI_TableEntry* this) {
	UI_Button* button = (UI_Button*) this->data;
	char*      label;

	if (this->parent->selected == this->thisIndex) {
		label = SafeMalloc(strlen(button->label + 5));
		sprintf(label, "> %s <", button->label);
		return label;
	}
	else {
		return NewString(button->label);
	}
}

static void Render(GFX_Canvas* canvas, UI_TableEntry* this) {
	UI_Button* button    = (UI_Button*) this->data;
	char*      label     = GetLabel(this);

	Vec2 labelSize = GFX_TextSize(button->font, label);
	Vec2 labelPos  = (Vec2) {
		this->rect.x + (this->rect.w / 2) - (labelSize.x / 2),
		this->rect.y + (this->rect.h / 2) - (labelSize.y / 2)
	};

	GFX_DrawText(button->font, canvas, label, labelPos.x, labelPos.y);
	GFX_DrawRect(canvas, this->rect, 0xFFFFFFFF);
	free(label);
}

static int MinSize(UI_TableEntry* this) {
	UI_Button* button = (UI_Button*) this->data;
	char*      label  = GetLabel(this);
	int        ret    = GFX_TextSize(button->font, label).x + button->font->charWidth;

	free(label);
	return ret;
}

UI_TableEntry UI_NewButton(
	UI_ButtonStyle style, GFX_Font* font, const char* label, UI_ButtonAction onClick
) {
	UI_TableEntry ret;
	ret.data        = (void*) NEW(UI_Button);
	ret.selectable  = true;
	ret.free        = &Free;
	ret.handleEvent = &HandleEvent;
	ret.onAction    = &OnAction;
	ret.render      = &Render;
	ret.minSize     = &MinSize;
	
	UI_Button* button = (UI_Button*) ret.data;
	button->style     = style;
	button->label     = label;
	button->onClick   = onClick;
	button->font      = font;
	return ret;
}
