#include "app.h"
#include "game.h"
#include "input.h"
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

	app.font = GFX_LoadFont("assets/font.bmp", 8, 8);
	GFX_LoadImage(&app.uiTexture, "assets/ui.bmp");

	app.scenes[APPSCENE_TITLESCREEN] = TitlescreenScene();
	app.scenes[APPSCENE_GAME]        = GameScene();
	app.scene = app.scenes + APPSCENE_TITLESCREEN;
	app.scene->init();

	app.deltaTime = 0.0;
	app.fps       = 0.0;
	app.ticks     = 0;

	Input_Init();
	Input_BindKey(ACTION_CAMERA_FORWARD,     SDL_SCANCODE_W);
	Input_BindKey(ACTION_CAMERA_LEFT,        SDL_SCANCODE_A);
	Input_BindKey(ACTION_CAMERA_RIGHT,       SDL_SCANCODE_D);
	Input_BindKey(ACTION_CAMERA_BACKWARDS,   SDL_SCANCODE_S);
	Input_BindKey(ACTION_CAMERA_LOOK_LEFT,   SDL_SCANCODE_LEFT);
	Input_BindKey(ACTION_CAMERA_LOOK_RIGHT,  SDL_SCANCODE_RIGHT);
	Input_BindKey(ACTION_CAMERA_LOOK_UP,     SDL_SCANCODE_UP);
	Input_BindKey(ACTION_CAMERA_LOOK_DOWN,   SDL_SCANCODE_DOWN);
	Input_BindKey(ACTION_TABLE_SELECT_LEFT,  SDL_SCANCODE_LEFT);
	Input_BindKey(ACTION_TABLE_SELECT_RIGHT, SDL_SCANCODE_RIGHT);
	Input_BindKey(ACTION_TABLE_SELECT_UP,    SDL_SCANCODE_UP);
	Input_BindKey(ACTION_TABLE_SELECT_DOWN,  SDL_SCANCODE_DOWN);
	Input_BindKey(ACTION_TABLE_SELECT,       SDL_SCANCODE_RETURN);
}

void App_Free() {
	GFX_FreeScreen(&app.screen);
	app.scene->free();

	Input_Free();
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
