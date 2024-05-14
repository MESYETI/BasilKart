#include <stdio.h>
#include "ui.h"
#include "app.h"
#include "map.h"
#include "util.h"
#include "game.h"
#include "ui/label.h"
#include "ui/button.h"
#include "gfx/image.h"
#include "gfx/canvas.h"
#include "titlescreen.h"

static UI_Manager  ui;
static UI_Element* tableElem;
static UI_Table*   table;
static Map         map;
static Camera      camera;

static void Free();

static void PlayButton(UI_TableEntry* this) {
	UNUSED(this);

	App* app   = App_Instance();
	app->scene = app->scene + APPSCENE_GAME;
	app->scene->init();
	Free();
}

static void ExitButton(UI_TableEntry* this) {
	UNUSED(this);
	exit(0);
}

static void Init(void) {
	App* app = App_Instance();
	ui       = UI_NewManager();
	Map_Init(&map, 32, 32);
	GFX_LoadImage(&map.skybox, "assets/sky.bmp");

	camera.pos  = (FVec3) {10.0, 16.0, 0.5};
	camera.dirH = 0.0;
	camera.dirV = 0.0;

	UI_AddManagerElement(&ui, UI_NewTable(0, 160, 136, 64, 1));
	tableElem = UI_GetLastElement(&ui);
	table     = (UI_Table*) tableElem->data;
	UI_SetTableElementSize(tableElem, 180, 16);
	UI_SetTableMargin(tableElem, 8, 8);
	UI_CenterElement(tableElem);

	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "Grand prix", &PlayButton
	));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "Time trial", &PlayButton
	));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "Exit", &ExitButton	
	));

	UI_UpdateTableHeight(tableElem);
	UI_InitTableSelection(tableElem);
}

static void Free(void) {
	UI_FreeManager(&ui);
}

static void Update(void) {

}

static void Render(GFX_Canvas* canvas) {
	GFX_ClearCanvas(canvas, 0, 0, 0);
	Map_Render(&map, canvas, camera);
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
