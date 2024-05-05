#include <stdio.h>
#include "app.h"
#include "util.h"
#include "gfx/canvas.h"
#include "titlescreen.h"

static void Init(void) {
	
}

static void Free(void) {
	
}

static void Update(void) {
	GFX_Canvas* canvas = &App_Instance()->canvas;
	GFX_ClearCanvas(canvas, 0, 0, 255);
}

static void Render(GFX_Canvas* canvas) {
	UNUSED(canvas);
}

static void HandleEvent(SDL_Event* e) {
	UNUSED(e);
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
