#include <math.h>
#include "types.h"
#include "util.h"

int GetDistance(Vec2 start, Vec2 end) {
	FVec2 difference = (FVec2) {
		fabs(((float) start.x) - ((float) end.x)),
		fabs(((float) start.y) - ((float) end.y))
	};

	return (int) sqrtf(pow(difference.x, 2) + pow(difference.y, 2));
}

FVec2 LerpVec2(Vec2 start, Vec2 end, double t) {
	return (FVec2) {
		Lerp(start.x, end.x, t),
		Lerp(start.y, end.y, t)
	};
}
