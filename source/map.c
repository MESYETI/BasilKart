#include "map.h"
#include "safe.h"
#include "util.h"
#include "game.h"
#include "types.h"
#include "constants.h"
#include "gfx/image.h"

static const int centerY = APP_WIN_HEIGHT / 2;
static double    maxDist = 100;

static double rayDirMap[APP_WIN_WIDTH];

void Map_Init(Map* map, int width, int height) {
	map->width  = width;
	map->height = height;

	for (int i = 0; i < APP_WIN_WIDTH; ++ i) {
		rayDirMap[i] = RadToDeg(atan2(
			DegToRad(i - APP_WIN_WIDTH / 2),
			DegToRad(APP_WIN_WIDTH / 2 / tan(DegToRad(APP_FOV / 2)))
		));
	}

	printf("0 = %f\n", rayDirMap[0]);

	GFX_LoadImage(&map->texture, "assets/tiles.bmp");

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

static GFX_Pixel FogifyPixel(GFX_Pixel pixel, double distance) {
	GFX_Pixel fogColour = GFX_ColourToPixel(0x24, 0x9F, 0xDE, 0xFF);
	double    fogDist   = maxDist;
	double    fog       = distance / fogDist;

	if (fog > 1) {
		fog = 1;
	}

	fog  = 1 - fog;
	fog *= fog;
	return GFX_LerpPixel(fogColour, pixel, fog);
}

void Map_RenderSprite(GFX_Canvas* canvas, Camera camera, MapSprite* sprite) {
	double angle  = RadToDeg(atan2(sprite->y - camera.pos.y, sprite->x - camera.pos.x));
	angle        -= camera.dirH;

	while (angle < -180) angle += 360;
	while (angle >= 180) angle -= 360;

	if ((angle > 45.0) || (angle < -45.0)) return;

	double distance = sqrt(
		pow(camera.pos.y - sprite->y, 2) + pow(camera.pos.x - sprite->x, 2)
	);

	int x = (int) RadToDeg(
		tan(DegToRad(angle)) *
		DegToRad(APP_WIN_WIDTH / 2 / tan(DegToRad(APP_FOV / 2)))
	) + APP_WIN_WIDTH / 2; // complicated math by lurnie

	// Fix fisheye
	distance *= CosDeg(rayDirMap[x]);

	double h = 1;
	// height of sprite in the world, 1 is player height, 0.5 is half player height...
	double z = 0; // z position of sprite in the world, 0 is on the ground (player feet),
	             // 0.5 is in the center of the screen...
	//double unit = (double) canvas->height / distance;
	double unit = (double) centerY * 2 / distance;

	GFX_Point lineStart = {
		(int) x, round((double) centerY - (h - camera.pos.z + z) * unit) + camera.dirV
	};

	GFX_VLine(
		canvas, x, lineStart.y, round(unit * h),
		FogifyPixel(GFX_ColourToPixel(255, 255, 255, 255), distance)
	);
}

void Map_Render(Map* map, GFX_Canvas* canvas, Camera camera) {
	// draw skybox
	GFX_Rect skyboxDest = {0, 0, 320, APP_WIN_HEIGHT}; /* draw on entire window temporarily */
	GFX_Rect skyboxSrc  = {0, 0, 320, 92};
	GFX_BlitCanvas(canvas, &map->skybox, &skyboxDest, &skyboxSrc);

	for (size_t x = 0; x < APP_WIN_WIDTH; ++ x) {
		for (int y = 0; y <= centerY - camera.dirV; ++ y) {
			double direction = rayDirMap[x] + camera.dirH;
			double distance  = ((double) (APP_WIN_HEIGHT)) / ((double) y) * (camera.pos.z);
			distance        /= CosDeg(camera.dirH - direction);

			if (distance > maxDist) {
				continue;
			}

			FVec2 pixelPos = {
				CosDeg(direction) * distance + camera.pos.x + 0.5,
				SinDeg(direction) * distance + camera.pos.y + 0.5
			};
			Vec2 tilePos = {
				floor(pixelPos.x), floor(pixelPos.y)
			};

			FVec2 tilePixel = {
				pixelPos.x - ((double) tilePos.x),
				pixelPos.y - ((double) tilePos.y)
			};
			Vec2 pixel      = {x, y + (centerY) - 1 + camera.dirV};
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
				FogifyPixel(
					GFX_GetPixel(&map->texture, pixelIndex.x, pixelIndex.y), distance
				)
			);
		}
	}

	// draw test sprite
	MapSprite sprite;
	sprite.x = 10.0;
	sprite.y = 10.0;
	Map_RenderSprite(canvas, camera, &sprite);

	/*if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP])
		rotY -= 3;
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN])
		rotY += 3;

	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q])
		cameraZ -= 0.05;
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E])
		cameraZ += 0.05;

	if (cameraZ < 0.05)
		cameraZ = 0.05;*/ // terrible code by LordOfTrident
}
