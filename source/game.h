#ifndef BK_GAME_H
#define BK_GAME_H

#include "map.h"
#include "types.h"
#include "scene.h"

struct Camera {
	FVec3  pos;
	double dirH;
	double dirV;
};

typedef struct Camera Camera;

typedef struct {
	Camera camera;
	Map    map;
} Game;

Scene GameScene(void);

#endif
