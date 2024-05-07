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

	app.deltaTime = 0.0;
	app.fps       = 0.0;
	app.ticks     = 0;
}

void App_Free() {
	GFX_FreeScreen(&app.screen);
	app.scene->free();
}

void App_Update() {
	static uint64_t deltaNow  = 0.0;
	static uint64_t deltaLast = 0.0;
	static double   lastFps   = 0.0;

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

	deltaLast     = deltaNow;
	deltaNow      = SDL_GetPerformanceCounter();
	app.deltaTime =
		(double) ((deltaNow - deltaLast) * 1000 / (double) SDL_GetPerformanceFrequency());
	app.fps = 1000 / app.deltaTime;

	if (app.ticks % 60 == 0) {
		lastFps = app.fps;
	}

	GFX_DrawText(&app.font, &app.canvas, "Basil Kart BETA", 1, 1);

	char infoText[50];
	sprintf(infoText, "FPS: %d", (int) round(lastFps));
	GFX_DrawText(&app.font, &app.canvas, infoText, 1, 10);

	GFX_RenderScreen(&app.screen, &app.canvas);

	++ app.ticks;
}
