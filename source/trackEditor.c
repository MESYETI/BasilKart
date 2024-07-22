#include <stdio.h>
#include "ui.h"
#include "app.h"
#include "map.h"
#include "game.h"
#include "util.h"
#include "input.h"
#include "ui/label.h"
#include "constants.h"
#include "gfx/image.h"
#include "gfx/canvas.h"
#include "trackEditor.h"

enum {
	TRACKEDITOR_VIEW_2D,
	TRACKEDITOR_VIEW_3D
};

static Map         map;
static Camera      camera;
static UI_Manager  ui;
static UI_Table*   table;
static UI_Element* tableElem;
static int         view;
static uint16_t    tile;

static void Init(void) {
	App* app = App_Instance();

	Map_Init(&map, 32, 32);
	GFX_LoadImage(&map.skybox, "assets/sky.bmp");
	GFX_LoadImage(&map.clouds, "assets/clouds.bmp");

	camera = (Camera) {(FVec3) {1.5, 0.0, 0.5}, 0.0, 0.0};

	ui = UI_NewManager();
	UI_AddManagerElement(&ui, UI_NewTable(0, 0, APP_WIN_WIDTH, 16, 1));
	tableElem = UI_GetLastElement(&ui);
	table     = (UI_Table*) tableElem->data;
	UI_SetTableElementSize(tableElem, APP_WIN_WIDTH, 16);
	UI_SetTableMargin(tableElem, 1, 1);
	table->slices.x = 3;

	UI_AddTableEntry(tableElem, UI_NewLabel(&app->font, "Track Editor"));

	table->columns = table->length;
	UI_UpdateElementRects(tableElem);
	UI_UpdateTableHeight(tableElem);

	tile = 1;
}

static void Free(void) {
	UI_FreeManager(&ui);
}

static void Update(void) {
	App*   app    = App_Instance();
	double moveBy = app->deltaTime * 3.0;

	if (Input_ActionActive(ACTION_VIEW_SCROLL_FAST))  moveBy        *= 2.0;
	if (Input_ActionActive(ACTION_VIEW_SCROLL_UP))    camera.pos.y -= moveBy;
	if (Input_ActionActive(ACTION_VIEW_SCROLL_DOWN))  camera.pos.y += moveBy;
	if (Input_ActionActive(ACTION_VIEW_SCROLL_LEFT))  camera.pos.x -= moveBy;
	if (Input_ActionActive(ACTION_VIEW_SCROLL_RIGHT)) camera.pos.x += moveBy;

	if (camera.pos.x < 0.0)                       camera.pos.x = 0.0;
	if (camera.pos.y < (-16.0 / APP_TILE_HEIGHT)) camera.pos.y = -16.0 / APP_TILE_HEIGHT;

	// TODO: max
}

static void Render(GFX_Canvas* canvas) {
	GFX_ClearCanvas(canvas, 0, 0, 0);
	Map_Render2D(&map, canvas, (Camera2D) {camera.pos.x, camera.pos.y});
	UI_RenderManager(&ui, canvas);

	// render selected tile
	GFX_Rect destRect;
	destRect.x = APP_WIN_WIDTH - (APP_TILE_WIDTH / 4) - 3;
	destRect.y = 19;
	destRect.w = APP_TILE_WIDTH / 4;
	destRect.h = APP_TILE_HEIGHT / 4;

	GFX_Rect srcRect;
	srcRect.x = map.tileDefs[tile].textureX * APP_TILE_WIDTH;
	srcRect.y = map.tileDefs[tile].textureY * APP_TILE_HEIGHT;
	srcRect.w = APP_TILE_WIDTH;
	srcRect.h = APP_TILE_HEIGHT;

	GFX_BlitCanvas(canvas, &map.texture, &destRect, &srcRect);
	GFX_DrawRect(canvas, destRect, GFX_ColourToPixel(255, 255, 255, 255));

	// draw a shadow
	GFX_HLine(
		canvas, destRect.x + 1, destRect.y + destRect.h, destRect.w,
		GFX_ColourToPixel(0, 0, 0, 128)
	);
	GFX_VLine(
		canvas, destRect.x + destRect.w, destRect.y + 1, destRect.h,
		GFX_ColourToPixel(0, 0, 0, 128)
	);
}

static void HandleEvent(SDL_Event* e) {
	UNUSED(e);
}

Scene TrackEditorScene(void) {
	return (Scene) {
		.init        = &Init,
		.free        = &Free,
		.update      = &Update,
		.render      = &Render,
		.handleEvent = &HandleEvent
	};
}
