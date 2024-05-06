#ifndef BK_MAP_H
#define BK_MAP_H

#include "types.h"
#include "gfx/canvas.h"

struct Camera;

typedef struct {
	double     x, y, z;
	GFX_Canvas canvas;
} MapSprite;

typedef struct {
	uint16_t id;
	uint8_t  rotation;
} MapTile;

typedef struct {
	uint16_t textureX;
	uint16_t textureY;
} MapTileDef;

typedef struct {
	GFX_Canvas texture;
	MapTileDef tileDefs[512];
	MapTile*   tiles;
	int        width;
	int        height;
	GFX_Canvas skybox;
} Map;

void     Map_Init(Map* map, int width, int height);
MapTile* Map_GetTile(Map* map, int x, int y);
void     Map_RenderSprite(GFX_Canvas* canvas, struct Camera camera, MapSprite* sprite);
void     Map_Render(Map* map, GFX_Canvas* canvas, struct Camera camera);

#endif
