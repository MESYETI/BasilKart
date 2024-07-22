#ifndef BK_TYPES_H
#define BK_TYPES_H

typedef struct {
	int x, y;
} Vec2;

typedef struct {
	double x, y;
} FVec2;

typedef struct {
	double x, y, z;
} FVec3;

int GetDistance(Vec2 start, Vec2 end);
FVec2 LerpVec2(Vec2 start, Vec2 end, double t);

#endif
