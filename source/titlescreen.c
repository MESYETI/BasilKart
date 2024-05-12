#include <stdio.h>
#include "app.h"
#include "util.h"
#include "ui.h"
#include "ui/label.h"
#include "ui/button.h"
#include "gfx/canvas.h"
#include "titlescreen.h"

static UI_Manager  ui;
static UI_Element* tableElem;
static UI_Table*   table;

static void Init(void) {
	App* app = App_Instance();
	ui       = UI_NewManager();

	UI_AddManagerElement(&ui, UI_NewTable(50, 50, 180, 16, 2));
	tableElem = UI_GetLastElement(&ui);
	table     = (UI_Table*) tableElem->data;
	UI_SetTableElementSize(tableElem, 180, 16);

	UI_AddTableEntry(tableElem, UI_NewLabel(&app->font, "label"));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "button I", NULL)
	);
	UI_AddTableEntry(tableElem, UI_NewLabel(&app->font, "label"));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "button II", NULL)
	);
	UI_AddTableEntry(tableElem, UI_NewLabel(&app->font, "label"));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "button III", NULL)
	);
	UI_AddTableEntry(tableElem, UI_NewLabel(&app->font, "label"));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "button IV", NULL)
	);
}

static void Free(void) {
	UI_FreeManager(&ui);
}

static void Update(void) {
	GFX_Canvas* canvas = &App_Instance()->canvas;
	GFX_ClearCanvas(canvas, 0, 0, 255);
}

static void Render(GFX_Canvas* canvas) {
	UI_RenderManager(&ui, canvas);
}

static void HandleEvent(SDL_Event* e) {
	UI_ManagerHandleEvent(&ui, e);
}

Scene TitlescreenScene(void) {
	return (Scene) {
		.init        = &Init,
		.free        = &Free,
		.update      = &Update,
		.render      = &Render,
		.handleEvent = &HandleEvent
	};
}
