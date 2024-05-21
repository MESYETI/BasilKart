#ifndef BK_SCENE_H
#define BK_SCENE_H

#include "util.h"
#include "gfx/canvas.h"

typedef struct {
	FUNCTION_POINTER(void, init, void);
	FUNCTION_POINTER(void, free, void);
	FUNCTION_POINTER(void, update, void);
	FUNCTION_POINTER(void, render, GFX_Canvas*);
	FUNCTION_POINTER(void, handleEvent, SDL_Event*);
} Scene;

#endif
