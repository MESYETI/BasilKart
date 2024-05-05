#include <stdio.h>
#include "app.h"
#include "map.h"
#include "game.h"
#include "util.h"
#include "gfx/image.h"
#include "gfx/canvas.h"

Game game;

static void Init(void) {
	Map_Init(&game.map, 32, 32);
	GFX_LoadImage(&game.map.skybox, "assets/sky.bmp");

	game.camDir = 0.0;
	game.camera = (FVec2) {12.0, 16.0};
}

static void Free(void) {
	
}

static void Update(void) {
	game.camDir += 0.5;
}

static void Render(GFX_Canvas* canvas) {
	Map_Render(&game.map, canvas, game.camera, game.camDir);
}

static void HandleEvent(SDL_Event* e) {
	UNUSED(e);
}

Scene GameScene(void) {
	return (Scene) {
		.init        = &Init,
		.free        = &Free,
		.update      = &Update,
		.render      = &Render,
		.handleEvent = &HandleEvent
	};
}
