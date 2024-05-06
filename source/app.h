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
	APPSCENE_AMOUNT
} AppScene;

typedef struct {
	bool       running;
	GFX_Screen screen;
	Scene      scenes[APPSCENE_AMOUNT];
	Scene*     scene;
	GFX_Canvas canvas;
	GFX_Font   font;
} App;

App* App_Instance(void);

void App_Init();
void App_Free();
void App_Update();

#endif
