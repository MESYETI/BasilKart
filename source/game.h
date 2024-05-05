#ifndef BK_GAME_H
#define BK_GAME_H

#include "map.h"
#include "types.h"
#include "scene.h"

typedef struct {
	FVec2  camera;
	double camDir;
	Map    map;
} Game;

Scene GameScene(void);

#endif
