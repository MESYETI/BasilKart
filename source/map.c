#include <stdio.h>
#include "app.h"
#include "map.h"
#include "safe.h"
#include "util.h"
#include "game.h"
#include "input.h"
#include "types.h"
#include "constants.h"
#include "gfx/image.h"

static const int   centerY     = APP_WIN_HEIGHT / 2;
static double      maxDist     = 100;
static const float cloudsScale = 50;
static const float cloudsY     = 10;
static const float cloudsSpeed = 0.0002;
static GFX_Canvas  testSprite;

static double rayDirMap[APP_WIN_WIDTH];

void Map_Init(Map* map, int width, int height) {
	map->width  = width;
	map->height = height;

	map->cloudsOffset = 0;

	for (int i = 0; i < APP_WIN_WIDTH; ++ i) {
		rayDirMap[i] = atan2(
			DegToRad(i - APP_WIN_WIDTH / 2),
			DegToRad(APP_WIN_WIDTH / 2 / tan(DegToRad(APP_FOV / 2)))
		);
	}

	GFX_LoadImage(&map->texture, "assets/tiles.bmp");
	GFX_LoadImage(&testSprite, "test.bmp");

	map->tiles = SafeMalloc(sizeof(MapTile) * width * height);

	map->tileDefs[0] = (MapTileDef) { // grass
		.textureX = 0, .textureY = 0
	};
	map->tileDefs[1] = (MapTileDef) { // straight road
		.textureX = 1, .textureY = 0
	};

	for (int x = 0; x < width; ++ x) {
		for (int y = 0; y < height; ++ y) {
			if (x == 4) {
				Map_GetTile(map, x, y)->id = 1;
			}
			else {
				Map_GetTile(map, x, y)->id = 0;
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

// just for testing stuff
Vec2 Map_3DTo2D(Camera camera, FVec3 pos) {
	double angle  = atan2(pos.y - camera.pos.y, pos.x - camera.pos.x);
	angle        -= camera.dirH;

	while (angle <  DegToRad(-180)) angle += DegToRad(360);
	while (angle >= DegToRad(180))  angle -= DegToRad(360);

	if ((angle > DegToRad(45.0)) || (angle < DegToRad(-45.0))) return (Vec2) {0, 0};

	double distance = sqrt(
		pow(camera.pos.y - pos.y, 2) + pow(camera.pos.x - pos.x, 2)
	) * 4;

	int x = (int) RadToDeg(
		tan(angle) *
		DegToRad(APP_WIN_WIDTH / 2 / tan(DegToRad(APP_FOV / 2)))
	) + APP_WIN_WIDTH / 2; // complicated math by lurnie

	// Fix fisheye
	distance *= cos(rayDirMap[x]);

	double unit = (double) centerY * 2 / distance;

	return (Vec2) {
		x, round((double) centerY - pos.z * unit) + camera.dirV
	};
}

void Map_RenderTriangle(
	GFX_Canvas* canvas, Camera camera, FVec3 p1, FVec3 p2, FVec3 p3, GFX_Pixel col
) {
	GFX_Triangle(
		canvas, Map_3DTo2D(camera, p1), Map_3DTo2D(camera, p2), Map_3DTo2D(camera, p3),
		col
	);
}

void Map_RenderSprite(GFX_Canvas* canvas, Camera camera, MapSprite* sprite) {
	double angle  = atan2(sprite->y - camera.pos.y, sprite->x - camera.pos.x);
	angle        -= camera.dirH;

	while (angle <  DegToRad(-180)) angle += DegToRad(360);
	while (angle >= DegToRad(180))  angle -= DegToRad(360);

	if ((angle > DegToRad(45.0)) || (angle < DegToRad(-45.0))) return;

	double distance = sqrt(
		pow(camera.pos.y - sprite->y, 2) + pow(camera.pos.x - sprite->x, 2)
	) * 4;

	int x = (int) RadToDeg(
		tan(angle) *
		DegToRad(APP_WIN_WIDTH / 2 / tan(DegToRad(APP_FOV / 2)))
	) + APP_WIN_WIDTH / 2; // complicated math by lurnie

	// Fix fisheye
	distance *= cos(rayDirMap[x]);

	double h = 1;
	// height of sprite in the world, 1 is player height, 0.5 is half player height...
	//double z = 0; // z position of sprite in the world, 0 is on the ground (player feet),
	             // 0.5 is in the center of the screen...
	//double unit = (double) canvas->height / distance;
	double unit = (double) centerY * 2 / distance;

	/*int lineStart =
		round((double) centerY - (h - camera.pos.z + sprite->z) * unit) + camera.dirV;

	GFX_VLine(
		canvas, x, lineStart, round(unit * h),
		FogifyPixel(GFX_ColourToPixel(255, 255, 255, 255), distance)
	);*/

	int spriteStartY =
		round((double) centerY - (h - camera.pos.z + sprite->z) * unit) + camera.dirV;
	int spriteHeight = round(unit * h);

	GFX_Rect rect;
	rect.h = spriteHeight;
	rect.w = (int) (
		((double) spriteHeight / sprite->canvas.height) * (double) sprite->canvas.width
	);
	rect.x = x - (rect.w / 2);
	rect.y = spriteStartY;

	GFX_BlitCanvas(canvas, &sprite->canvas, &rect, NULL);
}

void Map_Render(Map* map, GFX_Canvas* canvas, Camera camera) {
	// draw skybox
	App*     app        = App_Instance();
	GFX_Rect skyboxDest = {0, 0, 320, APP_WIN_HEIGHT}; /* draw on entire window temporarily */
	GFX_Rect skyboxSrc  = {0, 0, 320, 92};
	GFX_BlitCanvas(canvas, &map->skybox, &skyboxDest, &skyboxSrc);

	for (size_t x = 0; x < APP_WIN_WIDTH; ++ x) {
		for (int y = 0; y <= centerY - camera.dirV; ++ y) {
			double direction = rayDirMap[x] + camera.dirH;
			double distance  = ((double) (APP_WIN_HEIGHT)) / ((double) y) * (camera.pos.z);
			distance        /= cos(camera.dirH - direction);
			distance        *= 0.25;

			if (distance > maxDist) {
				continue;
			}

			FVec2 pixelPos = {
				cos(direction) * distance + camera.pos.x + 0.5,
				sin(direction) * distance + camera.pos.y + 0.5
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

			// TODO: remove this, its just for seeing if basil kart has the far lands
			if (pixelIndex.x < 0) pixelIndex.x = -pixelIndex.x;
			if (pixelIndex.y < 0) pixelIndex.y = -pixelIndex.y;
			pixelIndex.x %= APP_TILE_WIDTH;
			pixelIndex.y %= APP_TILE_HEIGHT;

			if (
				(tilePos.x < 0) || (tilePos.y < 0) || (tilePos.x >= map->width) ||
				(tilePos.y >= map->height)
			) {
				//pixelIndex.x += 64;
			}
			else {
				MapTileDef* tileDef = &map->tileDefs[
					Map_GetTile(map, tilePos.x, tilePos.y)->id
				];

				pixelIndex.x += tileDef->textureX * APP_TILE_WIDTH;
				pixelIndex.y += tileDef->textureY * APP_TILE_HEIGHT;
			}

			if (app->settings.renderFog) {
				GFX_DrawPixel(
					canvas, pixel.x, pixel.y,
					FogifyPixel(
						GFX_GetPixel(&map->texture, pixelIndex.x, pixelIndex.y), 
						distance
					)
				);
			}
			else {
				GFX_DrawPixel(
					canvas, pixel.x, pixel.y,
					GFX_GetPixel(&map->texture, pixelIndex.x, pixelIndex.y)
				);
			}
		}
	}

	if (!app->settings.renderSky) goto afterSky;

	for (size_t x = 0; x < APP_WIN_WIDTH; ++ x) {
		for (int y = 0; y <= centerY + camera.dirV; ++ y) {
			double direction = rayDirMap[x] + camera.dirH;
			double distance  = ((double) (APP_WIN_HEIGHT)) / ((double) y) * (camera.pos.z + cloudsY);
			distance        /= cos(camera.dirH - direction);
			//distance        *= 0.25;

			if (distance > maxDist) {
				continue;
			}

			FVec2 pixelPos = {
				(cos(direction) * distance + camera.pos.x + 0.5) / cloudsScale + map->cloudsOffset,
				(sin(direction) * distance + camera.pos.y + 0.5) / cloudsScale + map->cloudsOffset
			};
			Vec2 tilePos = {
				floor(pixelPos.x), floor(pixelPos.y)
			};

			FVec2 tilePixel = {
				pixelPos.x - ((double) tilePos.x),
				pixelPos.y - ((double) tilePos.y)
			};
			Vec2 pixel      = {x, (centerY - y) + camera.dirV};
			Vec2 pixelIndex = {
				(int) (tilePixel.x * ((double) map->clouds.width)),
				(int) (tilePixel.y * ((double) map->clouds.height))
			};

			// TODO: remove this, its just for seeing if basil kart has the far lands
			if (pixelIndex.x < 0) pixelIndex.x = -pixelIndex.x;
			if (pixelIndex.y < 0) pixelIndex.y = -pixelIndex.y;
			pixelIndex.x %= map->clouds.width;
			pixelIndex.y %= map->clouds.height;

			if (app->settings.renderFog) {
				GFX_DrawPixel(
					canvas, pixel.x, pixel.y,
					FogifyPixel(
						GFX_GetPixel(&map->clouds, pixelIndex.x, pixelIndex.y), 
						distance
					)
				);
			}
			else {
				GFX_DrawPixel(
					canvas, pixel.x, pixel.y,
					GFX_GetPixel(&map->clouds, pixelIndex.x, pixelIndex.y)
				);
			}
		}
	}
	// TODO: Multiply cloudsSpeed by delta time
	// ok rei
	map->cloudsOffset += cloudsSpeed;

	// draw test sprite
	MapSprite sprite;

	afterSky: // C wants me to put it here for some reason

	sprite.x      = 10.0;
	sprite.y      = 10.0;
	sprite.z      = 0.0; // this wasn't here before but it worked fine?
	sprite.canvas = testSprite;
	Map_RenderSprite(canvas, camera, &sprite);

	Vec2 pixelPos = Map_3DTo2D(camera, (FVec3) {10.0, 10.0, 0.0});
	GFX_DrawPixel(canvas, pixelPos.x, pixelPos.y, GFX_ColourToPixel(255, 255, 255, 255));

	Map_RenderTriangle(
		canvas, camera,
		(FVec3) {10.0, 10.0, 10.0},
		(FVec3) {10.5, 10.0, 0.0},
		(FVec3) {11.0, 10.0, 10.0},
		0xFFFFFFFF
	);

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
	// ^ theres nothing terrible about that code yeti
	// shut up
}

void Map_Render2D(Map* map, GFX_Canvas* canvas, struct Camera2D camera) {
	Vec2 end;
	end.x = ((int) camera.x) + (APP_WIN_WIDTH / APP_TILE_WIDTH) + 2;
	end.y = ((int) camera.y) + (APP_WIN_HEIGHT / APP_TILE_HEIGHT) + 2;

	Vec2 start;
	start.x = camera.x > 0? (int) camera.x : 0;
	start.y = camera.y > 0? (int) camera.y : 0;

	Vec2 mousePos = Input_GetUser(0)->mousePos;

	map->selected = (Vec2) {-1, -1};

	for (int y = start.y; (y < end.y) && (y < map->height); ++ y) {
		for (int x = start.x; (x < end.x) && (x < map->width); ++ x) {
			GFX_Rect tileRect = (GFX_Rect) {
				(x * APP_TILE_WIDTH) - (int) (camera.x * APP_TILE_WIDTH),
				(y * APP_TILE_HEIGHT) - (int) (camera.y * APP_TILE_HEIGHT),
				APP_TILE_WIDTH,
				APP_TILE_HEIGHT
			};
			MapTile*    tile = Map_GetTile(map, x, y);
			MapTileDef* def  = &map->tileDefs[tile->id];
			// TODO: if i ever allow custom maps in multiplayer then this is a
			// vulnerability

			GFX_Rect textureSrc = (GFX_Rect) {
				def->textureX * APP_TILE_WIDTH,
				def->textureY * APP_TILE_HEIGHT,
				APP_TILE_WIDTH, APP_TILE_HEIGHT
			};
			GFX_BlitCanvas(canvas, &map->texture, &tileRect, &textureSrc);

			GFX_VLine(
				canvas, tileRect.x, tileRect.y, tileRect.h,
				GFX_ColourToPixel(255, 255, 255, 255)
			);
			GFX_HLine(
				canvas, tileRect.x, tileRect.y, tileRect.w,
				GFX_ColourToPixel(255, 255, 255, 255)
			);

			if (
				(mousePos.x >= tileRect.x) && (mousePos.y >= tileRect.y) &&
				(mousePos.x < tileRect.x + tileRect.w) &&
				(mousePos.y < tileRect.y + tileRect.h)
			) {
				map->selected = (Vec2) {x, y};
				GFX_DrawRect(canvas, tileRect, GFX_ColourToPixel(255, 0, 0, 255));
			}
		}
	}
}
