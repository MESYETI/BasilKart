#ifndef BK_UTIL_H
#define BK_UTIL_H

#define FUNCTION_POINTER(TYPE, NAME, ...) TYPE (*NAME)(__VA_ARGS__)
#define UNUSED(V) (void) V

#include <math.h>
double RadToDeg(double rad);
double DegToRad(double deg);
double CosDeg(double deg);
double SinDeg(double deg);

#endif
