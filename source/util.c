#include <string.h>
#include "util.h"
#include "safe.h"
#include "common.h"

double RadToDeg(double rad) {
	return rad * 180 / PI;
}

double DegToRad(double deg) {
	return deg * (PI / 180);
}

double Lerp(double a, double b, double t) {
    return a * (1.0 - t) + (b * t);
}

char* NewString(const char* src) {
	char* ret = SafeMalloc(strlen(src) + 1);
	strcpy(ret, src);
	return ret;
}
