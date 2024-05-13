#ifndef BK_UTIL_H
#define BK_UTIL_H

#include <math.h>

#define FUNCTION_POINTER(TYPE, NAME, ...) TYPE (*NAME)(__VA_ARGS__)
#define UNUSED(V) (void) V

double RadToDeg(double rad);
double DegToRad(double deg);
double CosDeg(double deg);
double SinDeg(double deg);
double Lerp(double a, double b, double t);
char*  NewString(const char* src);

#endif
