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

	game.camera.pos  = (FVec3) {12.0, 16.0, 0.5};
	game.camera.dirH = 0.0;
	game.camera.dirV = 0.0;
}

static void Free(void) {
	
}

static void Update(void) {
	const uint8_t* keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_W]) {
		game.camera.pos.x += cos(DegToRad(game.camera.dirH)) * 0.1;
		game.camera.pos.y += sin(DegToRad(game.camera.dirH)) * 0.1;
	}
	if (keyState[SDL_SCANCODE_A]) {
		game.camera.pos.x += cos(DegToRad(game.camera.dirH - 90)) * 0.1;
		game.camera.pos.y += sin(DegToRad(game.camera.dirH - 90)) * 0.1;
	}
	if (keyState[SDL_SCANCODE_S]) {
		game.camera.pos.x += cos(DegToRad(game.camera.dirH + 180)) * 0.1;
		game.camera.pos.y += sin(DegToRad(game.camera.dirH + 180)) * 0.1;
	}
	if (keyState[SDL_SCANCODE_D]) {
		game.camera.pos.x += cos(DegToRad(game.camera.dirH + 90)) * 0.1;
		game.camera.pos.y += sin(DegToRad(game.camera.dirH + 90)) * 0.1;
	}
	if (keyState[SDL_SCANCODE_LEFT]) {
		game.camera.dirH -= 3.0;

		while (game.camera.dirH < -180) {
			game.camera.dirH += 360.0;
		}
	}
	if (keyState[SDL_SCANCODE_RIGHT]) {
		game.camera.dirH += 3.0;

		while (game.camera.dirH > 180) {
			game.camera.dirH -= 360;
		}
	}
	if (keyState[SDL_SCANCODE_UP]) {
		game.camera.dirV += 3.0;

		if (game.camera.dirV > 180) {
			game.camera.dirV = 180;
		}
	}
	if (keyState[SDL_SCANCODE_DOWN]) {
		game.camera.dirV -= 3.0;

		if (game.camera.dirV < -180) {
			game.camera.dirV = -180;
		}
	}
}

static void Render(GFX_Canvas* canvas) {
	Map_Render(&game.map, canvas, game.camera);
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
