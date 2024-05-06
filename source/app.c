#include "app.h"
#include "game.h"
#include "constants.h"
#include "titlescreen.h"
#include "gfx/image.h"

static App app;

App* App_Instance(void) {
	return &app;
}

void App_Init() {
	app.running = true;
	GFX_InitScreen(&app.screen, APP_WIN_WIDTH, APP_WIN_HEIGHT);
	GFX_NewCanvas(&app.canvas, APP_WIN_WIDTH, APP_WIN_HEIGHT);

	app.scenes[APPSCENE_TITLESCREEN] = TitlescreenScene();
	app.scenes[APPSCENE_GAME]        = GameScene();
	app.scene = app.scenes + APPSCENE_GAME;
	app.scene->init();

	app.font = GFX_LoadFont("assets/font.bmp", 8, 8);
}

void App_Free() {
	GFX_FreeScreen(&app.screen);
	app.scene->free();
}

void App_Update() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT: {
				app.running = false;
				break;
			}
			default: {
				app.scene->handleEvent(&e);
			}
		}
	}

	app.scene->update();
	app.scene->render(&app.canvas);

	GFX_DrawText(&app.font, &app.canvas, "Basil Kart BETA", 1, 1);

	GFX_RenderScreen(&app.screen, &app.canvas);
}
