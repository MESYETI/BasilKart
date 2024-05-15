#include <stdio.h>
#include "app.h"
#include "map.h"
#include "game.h"
#include "util.h"
#include "input.h"
#include "gfx/image.h"
#include "gfx/canvas.h"

Game game;

static void Init(void) {
	Map_Init(&game.map, 32, 32);
	GFX_LoadImage(&game.map.skybox, "assets/sky.bmp");
	GFX_LoadImage(&game.map.clouds, "assets/clouds.bmp");

	game.camera.pos  = (FVec3) {12.0, 16.0, 0.5};
	game.camera.dirH = 0.0;
	game.camera.dirV = 0.0;
}

static void Free(void) {

}

static void Update(void) {
	if (Input_ActionActive(ACTION_CAMERA_FORWARD)) {
		game.camera.pos.x += cos(DegToRad(game.camera.dirH)) * 0.1;
		game.camera.pos.y += sin(DegToRad(game.camera.dirH)) * 0.1;
	}
	if (Input_ActionActive(ACTION_CAMERA_LEFT)) {
		game.camera.pos.x += cos(DegToRad(game.camera.dirH - 90)) * 0.1;
		game.camera.pos.y += sin(DegToRad(game.camera.dirH - 90)) * 0.1;
	}
	if (Input_ActionActive(ACTION_CAMERA_BACKWARDS)) {
		game.camera.pos.x += cos(DegToRad(game.camera.dirH + 180)) * 0.1;
		game.camera.pos.y += sin(DegToRad(game.camera.dirH + 180)) * 0.1;
	}
	if (Input_ActionActive(ACTION_CAMERA_RIGHT)) {
		game.camera.pos.x += cos(DegToRad(game.camera.dirH + 90)) * 0.1;
		game.camera.pos.y += sin(DegToRad(game.camera.dirH + 90)) * 0.1;
	}
	if (Input_ActionActive(ACTION_CAMERA_LOOK_LEFT)) {
		game.camera.dirH -= 3.0;

		while (game.camera.dirH < -180) {
			game.camera.dirH += 360.0;
		}
	}
	if (Input_ActionActive(ACTION_CAMERA_LOOK_RIGHT)) {
		game.camera.dirH += 3.0;

		while (game.camera.dirH > 180) {
			game.camera.dirH -= 360;
		}
	}
	if (Input_ActionActive(ACTION_CAMERA_LOOK_UP)) {
		game.camera.dirV += 3.0;

		if (game.camera.dirV > 180) {
			game.camera.dirV = 180;
		}
	}
	if (Input_ActionActive(ACTION_CAMERA_LOOK_DOWN)) {
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
