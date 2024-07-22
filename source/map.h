#ifndef BK_MAP_H
#define BK_MAP_H

#include "types.h"
#include "gfx/canvas.h"

struct Camera;
struct Camera2D;

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
	GFX_Canvas clouds;
	float      cloudsOffset;
	Vec2       selected;
} Map;

void     Map_Init(Map* map, int width, int height);
MapTile* Map_GetTile(Map* map, int x, int y);
Vec2     Map_3DTo2D(struct Camera camera, FVec3 pos);
void     Map_RenderTriangle(
	GFX_Canvas* canvas, struct Camera camera, FVec3 p1, FVec3 p2, FVec3 p3, GFX_Pixel col
);
void     Map_RenderSprite(GFX_Canvas* canvas, struct Camera camera, MapSprite* sprite);
void     Map_Render(Map* map, GFX_Canvas* canvas, struct Camera camera);
void     Map_Render2D(Map* map, GFX_Canvas* canvas, struct Camera2D camera);

#endif
