#include "map.h"
#include "safe.h"
#include "util.h"
#include "types.h"
#include "constants.h"
#include "gfx/image.h"

static double rayDirMap[APP_WIN_WIDTH];
static GFX_Canvas testSprite;

void Map_Init(Map* map, int width, int height) {
	map->width  = width;
	map->height = height;

	for (int i = 0; i < APP_WIN_WIDTH; ++ i) {
		rayDirMap[i] = RadToDeg(atan2(
			DegToRad(i - APP_WIN_WIDTH / 2),
			DegToRad(APP_WIN_WIDTH / 2 / tan(DegToRad(APP_FOV / 2)))
		));
	}

	GFX_LoadImage(&map->texture, "assets/tiles.bmp");
	GFX_LoadImage(&testSprite, "test.bmp");

	map->tiles = SafeMalloc(sizeof(MapTile) * width * height);

	map->tileDefs[0x00] = (MapTileDef) { // corner road
		.textureX = 0, .textureY = 0
	};
	map->tileDefs[0x10] = (MapTileDef) { // road V west
		.textureX = 1, .textureY = 0
	};
	map->tileDefs[0x11] = (MapTileDef) { // road V east
		.textureX = 1, .textureY = 1
	};
	map->tileDefs[0x12] = (MapTileDef) { // road H north
		.textureX = 1, .textureY = 2
	};
	map->tileDefs[0x13] = (MapTileDef) { // road H south
		.textureX = 1, .textureY = 3
	};
	map->tileDefs[0x20] = (MapTileDef) { // grass
		.textureX = 2, .textureY = 0
	};
	map->tileDefs[0x30] = (MapTileDef) { // road bounds V east
		.textureX = 3, .textureY = 0
	};
	map->tileDefs[0x31] = (MapTileDef) { // road bounds V west
		.textureX = 3, .textureY = 1
	};
	map->tileDefs[0x32] = (MapTileDef) { // road bounds H south
		.textureX = 3, .textureY = 2
	};
	map->tileDefs[0x33] = (MapTileDef) { // road bounds H north
		.textureX = 3, .textureY = 3
	};

	for (int x = 0; x < width; ++ x) {
		for (int y = 0; y < height; ++ y) {
			if (x == 12) {
				Map_GetTile(map, x, y)->id = 0x30;
			}
			else if (x == 13) {
				Map_GetTile(map, x, y)->id = 0x10;
			}
			else if (x == 14) {
				Map_GetTile(map, x, y)->id = 0x11;
			}
			else if (x == 15) {
				Map_GetTile(map, x, y)->id = 0x31;
			}
			else {
				Map_GetTile(map, x, y)->id = 0x20;
			}
		}
	}
}

MapTile* Map_GetTile(Map* map, int x, int y) {
	if ((x < 0) || (y < 0) || (x >= map->width) || (y >= map->height)) {
		fprintf(stderr, "Out of bounds %d %d\n", x, y);
		assert(0);
	}

	return &map->tiles[(y * map->width) + x];
}

void Map_DrawSprite(GFX_Canvas* canvas, FVec2 camera, double camDir, MapSprite* sprite) {
	double angle  = RadToDeg(atan2(camera.y - sprite->y, camera.x - sprite->x));
	angle        += camDir;
}

void Map_Render(Map* map, GFX_Canvas* canvas, FVec2 camera, double camDir) {
	int horizon = (APP_WIN_HEIGHT / 2) - (APP_WIN_HEIGHT / 8);

	for (size_t x = 0; x < APP_WIN_WIDTH; ++ x) {
		for (int y = 0; y < APP_WIN_HEIGHT - horizon; ++ y) {
			double direction = rayDirMap[x] + camDir;
			double distance  = ((double) (horizon)) / ((double) y);
			distance        /= CosDeg(camDir - direction);

			if (distance > 100) {
				continue;
			}

			FVec2 pixelPos = {
				CosDeg(direction) * distance + camera.x + 0.5,
				SinDeg(direction) * distance + camera.y + 0.5
			};
			Vec2 tilePos = {
				floor(pixelPos.x), floor(pixelPos.y)
			};

			FVec2 tilePixel = {
				pixelPos.x - ((double) tilePos.x),
				pixelPos.y - ((double) tilePos.y)
			};
			Vec2 pixel      = {x, y + (horizon)};
			Vec2 pixelIndex = {
				(int) (tilePixel.x * ((double) APP_TILE_WIDTH)),
				(int) (tilePixel.y * ((double) APP_TILE_HEIGHT))
			};

			if (
				(tilePos.x < 0) || (tilePos.y < 0) || (tilePos.x >= map->width) ||
				(tilePos.y >= map->height)
			) {
				pixelIndex.x += 64;
			}
			else {
				MapTileDef* tileDef = &map->tileDefs[
					Map_GetTile(map, tilePos.x, tilePos.y)->id
				];

				pixelIndex.x += tileDef->textureX * APP_TILE_WIDTH;
				pixelIndex.y += tileDef->textureY * APP_TILE_HEIGHT;
			}

			GFX_DrawPixel(
				canvas, pixel.x, pixel.y,
				GFX_GetPixel(&map->texture, pixelIndex.x, pixelIndex.y)
			);
		}
	}

	// draw skybox
	GFX_Rect skyboxDest = {0, 0, 320, 92};
	GFX_Rect skyboxSrc  = {0, 0, 320, 92};
	GFX_BlitCanvas(canvas, &map->skybox, &skyboxDest, &skyboxSrc);
}
