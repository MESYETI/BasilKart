#include <stdio.h>
#include "ui.h"
#include "app.h"
#include "map.h"
#include "util.h"
#include "input.h"
#include "ui/label.h"
#include "constants.h"
#include "gfx/image.h"
#include "gfx/canvas.h"
#include "trackEditor.h"

static TrackEditor this;

static void Init(void) {
	App* app = App_Instance();

	Map_Init(&this.map, 32, 32);
	GFX_LoadImage(&this.map.skybox, "assets/sky.bmp");
	GFX_LoadImage(&this.map.clouds, "assets/clouds.bmp");

	this.camera = (Camera2D) {1.5, 0.0};

	this.ui = UI_NewManager();
	UI_AddManagerElement(&this.ui, UI_NewTable(0, 0, APP_WIN_WIDTH, 16, 1));
	this.tableElem = UI_GetLastElement(&this.ui);
	this.table     = (UI_Table*) this.tableElem->data;
	UI_SetTableElementSize(this.tableElem, APP_WIN_WIDTH, 16);
	UI_SetTableMargin(this.tableElem, 1, 1);
	this.table->slices.x = 3;

	UI_AddTableEntry(this.tableElem, UI_NewLabel(&app->font, "Track Editor"));

	this.table->columns = this.table->length;
	UI_UpdateElementRects(this.tableElem);
	UI_UpdateTableHeight(this.tableElem);

	this.tile = 0x20;
}

static void Free(void) {
	UI_FreeManager(&this.ui);
}

static void Update(void) {
	App*   app    = App_Instance();
	double moveBy = app->deltaTime * 3.0;

	if (Input_ActionActive(ACTION_VIEW_SCROLL_FAST))  moveBy        *= 2.0;
	if (Input_ActionActive(ACTION_VIEW_SCROLL_UP))    this.camera.y -= moveBy;
	if (Input_ActionActive(ACTION_VIEW_SCROLL_DOWN))  this.camera.y += moveBy;
	if (Input_ActionActive(ACTION_VIEW_SCROLL_LEFT))  this.camera.x -= moveBy;
	if (Input_ActionActive(ACTION_VIEW_SCROLL_RIGHT)) this.camera.x += moveBy;

	if (this.camera.x < 0.0)                       this.camera.x = 0.0;
	if (this.camera.y < (-16.0 / APP_TILE_HEIGHT)) this.camera.y = -16.0 / APP_TILE_HEIGHT;

	// TODO: max
}

static void Render(GFX_Canvas* canvas) {
	GFX_ClearCanvas(canvas, 0, 0, 0);
	Map_Render2D(&this.map, canvas, this.camera);
	UI_RenderManager(&this.ui, canvas);

	// render selected tile
	GFX_Rect destRect;
	destRect.x = APP_WIN_WIDTH - APP_TILE_WIDTH - 3;
	destRect.y = 19;
	destRect.w = APP_TILE_WIDTH;
	destRect.h = APP_TILE_HEIGHT;

	GFX_Rect srcRect;
	srcRect.x = this.map.tileDefs[this.tile].textureX * APP_TILE_WIDTH;
	srcRect.y = this.map.tileDefs[this.tile].textureY * APP_TILE_HEIGHT;
	srcRect.w = APP_TILE_WIDTH;
	srcRect.h = APP_TILE_HEIGHT;

	GFX_BlitCanvas(canvas, &this.map.texture, &destRect, &srcRect);
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
