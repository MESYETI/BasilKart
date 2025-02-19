#include "label.h"
#include "../safe.h"

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

static void Render(GFX_Canvas* canvas, UI_TableEntry* this) {
	UI_Label* label     = (UI_Label*) this->data;
	Vec2      labelSize = GFX_TextSize(label->font, label->label);
	Vec2      labelPos  = (Vec2) {
		this->rect.x,
		this->rect.y + (this->rect.h / 2) - (labelSize.y / 2) - 1
	};

	GFX_DrawText(label->font, canvas, label->label, labelPos.x, labelPos.y);
}

static int MinSize(UI_TableEntry* this) {
	UI_Label* label  = (UI_Label*) this->data;

	return GFX_TextSize(label->font, label->label).x + label->font->charWidth;
}

UI_TableEntry UI_NewLabel(GFX_Font* font, const char* plabel) {
	UI_TableEntry ret;
	ret.data        = (void*) NEW(UI_Label);
	ret.selectable  = false;
	ret.free        = &Free;
	ret.handleEvent = &HandleEvent;
	ret.onAction    = &OnAction;
	ret.render      = &Render;
	ret.minSize     = &MinSize;

	UI_Label* label = (UI_Label*) ret.data;
	label->label   = plabel;
	label->font    = font;
	return ret;
}
