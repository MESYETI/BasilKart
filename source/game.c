#include <stdio.h>
#include "app.h"
#include "map.h"
#include "game.h"
#include "util.h"
#include "input.h"
#include "gfx/image.h"
#include "gfx/canvas.h"

static Game game;

static void Init(void) {
	Map_Init(&game.map, 32, 32);
	GFX_LoadImage(&game.map.skybox, "assets/sky.bmp");
	GFX_LoadImage(&game.map.clouds, "assets/clouds.bmp");

	game.camera.pos  = (FVec3) {2.0, 16.0, 0.5};
	game.camera.dirH = 0.0;
	game.camera.dirV = 0.0;

	//game.camera.pos.x = 900719925474.0;
}

static void Free(void) {

}

static void Update(void) {
	double speed = 0.025;

	if (Input_ActionActive(ACTION_CAMERA_FORWARD)) {
		game.camera.pos.x += cos(game.camera.dirH) * speed;
		game.camera.pos.y += sin(game.camera.dirH) * speed;
	}
	if (Input_ActionActive(ACTION_CAMERA_LEFT)) {
		game.camera.pos.x += cos(game.camera.dirH - DegToRad(90)) * speed;
		game.camera.pos.y += sin(game.camera.dirH - DegToRad(90)) * speed;
	}
	if (Input_ActionActive(ACTION_CAMERA_BACKWARDS)) {
		game.camera.pos.x += cos(game.camera.dirH + DegToRad(180)) * speed;
		game.camera.pos.y += sin(game.camera.dirH + DegToRad(180)) * speed;
	}
	if (Input_ActionActive(ACTION_CAMERA_RIGHT)) {
		game.camera.pos.x += cos(game.camera.dirH + DegToRad(90)) * speed;
		game.camera.pos.y += sin(game.camera.dirH + DegToRad(90)) * speed;
	}
	if (Input_ActionActive(ACTION_CAMERA_LOOK_LEFT)) {
		game.camera.dirH -= DegToRad(3.0);

		while (game.camera.dirH < DegToRad(-180)) {
			game.camera.dirH += DegToRad(360.0);
		}
	}
	if (Input_ActionActive(ACTION_CAMERA_LOOK_RIGHT)) {
		game.camera.dirH += DegToRad(3.0);

		while (game.camera.dirH > DegToRad(180)) {
			game.camera.dirH -= DegToRad(360);
		}
	}
	if (Input_ActionActive(ACTION_CAMERA_LOOK_UP)) {
		game.camera.dirV += 3.0;
	}
	if (Input_ActionActive(ACTION_CAMERA_LOOK_DOWN)) {
		game.camera.dirV -= 3.0;
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
