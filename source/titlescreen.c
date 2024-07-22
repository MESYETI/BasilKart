#include <math.h>
#include <stdio.h>
#include "ui.h"
#include "app.h"
#include "map.h"
#include "util.h"
#include "game.h"
#include "ui/label.h"
#include "constants.h"
#include "ui/button.h"
#include "gfx/image.h"
#include "gfx/canvas.h"
#include "titlescreen.h"
#include "trackEditor.h"

#define LOGO_HEIGHT 41

static UI_Manager  ui;
static UI_Element* tableElem;
static UI_Table*   table;
static Map         map;
static Camera      camera;

static void Free();

static void PlayButton(UI_TableEntry* this) {
	UNUSED(this);

	App* app   = App_Instance();
	app->scene = app->scenes + APPSCENE_GAME;
	app->scene->init();
	Free();
}

static void TrackEditorButton(UI_TableEntry* this) {
	UNUSED(this);

	App* app   = App_Instance();
	app->scene = app->scenes + APPSCENE_TRACK_EDITOR;
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
	GFX_LoadImage(&map.clouds, "assets/clouds.bmp");

	camera.pos  = (FVec3) {3.0, 16.0, 0.5};
	camera.dirH = 0.0;
	camera.dirV = 0.0;

	UI_AddManagerElement(&ui, UI_NewTable(0, 160, 152, 64, 1));
	tableElem = UI_GetLastElement(&ui);
	table     = (UI_Table*) tableElem->data;
	UI_SetTableElementSize(tableElem, 180, 16);
	UI_SetTableMargin(tableElem, 8, 8);
	UI_CenterElement(tableElem);

	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "Singleplayer", &PlayButton
	));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "Multiplayer", NULL
	));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "Track Editor", &TrackEditorButton
	));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "Options", NULL
	));
	UI_AddTableEntry(tableElem, UI_NewButton(
		UI_BUTTON_STYLE_ARROWS, &app->font, "Exit", &ExitButton
	));

	UI_UpdateElementRects(tableElem);
	UI_UpdateTableHeight(tableElem);
	UI_InitTableSelection(tableElem);

	tableElem->rect.y =
		(APP_WIN_HEIGHT / 2) - (tableElem->rect.h + (LOGO_HEIGHT * 2) + 8) / 2;
	tableElem->rect.y += (LOGO_HEIGHT * 2) + 8;
	UI_UpdateElementRects(tableElem);
}

static void Free(void) {
	UI_FreeManager(&ui);
}

static void Update(void) {

}

static void Render(GFX_Canvas* canvas) {
	App* app = App_Instance();

	GFX_ClearCanvas(canvas, 0, 0, 0);
	Map_Render(&map, canvas, camera);
	UI_RenderManager(&ui, canvas);

	// render logo
	GFX_Rect src;
	src.x = 0;
	src.y = 24;
	src.w = 59;
	src.h = 41;

	GFX_Rect dest;
	dest.w  = src.w * 2;
	dest.h  = src.h * 2;
	dest.x  = (APP_WIN_WIDTH / 2) - (dest.w / 2);
	dest.y  = (APP_WIN_HEIGHT / 2) - (tableElem->rect.h + (LOGO_HEIGHT * 2) + 8) / 2;
	//dest.y += (int) (sin(((double) app->ticks) * 0.075) * 10.0);
	GFX_BlitCanvas(canvas, &app->uiTexture, &dest, &src);
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
