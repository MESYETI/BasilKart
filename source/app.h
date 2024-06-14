#ifndef BK_APP_H
#define BK_APP_H

#include "scene.h"
#include "common.h"
#include "gfx/text.h"
#include "gfx/canvas.h"
#include "gfx/screen.h"

typedef enum {
	APPSCENE_TITLESCREEN = 0,
	APPSCENE_GAME,
	APPSCENE_TRACK_EDITOR,

	APPSCENE_AMOUNT
} AppScene;

typedef struct {
	bool renderSky;
	bool renderFog;
} AppSettings;

typedef struct {
	bool        running;
	GFX_Screen  screen;
	Scene       scenes[APPSCENE_AMOUNT];
	Scene*      scene;
	GFX_Canvas  canvas;
	GFX_Font    font;
	double      deltaTime;
	double      fps;
	uint64_t    ticks;
	GFX_Canvas  uiTexture;
	AppSettings settings;
} App;

App* App_Instance(void);

void App_Init();
void App_Free();
void App_Update();

#endif
