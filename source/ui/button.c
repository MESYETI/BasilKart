#include "button.h"
#include "../safe.h"

static void Free(UITableEntry* this) {
	UNUSED(this);
}

static void HandleEvent(UITableEntry* this, SDL_Event* e) {
	UNUSED(this);
}

static void OnAction(UITableEntry* this) {
	UNUSED(this);
}

static void Render(GFX_Canvas* canvas, UITableEntry* this) {
	UIButton* button = (UIButton*) this->data;
	
}

UITableEntry UI_NewButton(const char* label, UIButtonAction onClick) {
	UITableEntry ret;
	ret.data        = (void*) NEW(UIButton);
	ret.free        = &Free;
	ret.handleEvent = &HandleEvent;
	ret.onAction    = &OnAction;
	ret.render      = &Render;

	UIButton* button = (UIButton*) ret.data;
	button->label    = label;
	button->onClick  = onClick;
	return ret;
}
