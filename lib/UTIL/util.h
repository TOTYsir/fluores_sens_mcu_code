#ifndef __UTIL_H
#define __UTIL_H

#include "led.h"

#define WINDOW_RANGE_LEFT 0.4
#define WINDOW_RANGE_RIGHT 0.8

float get_norm(float values0[], float values1[]);
float calculate_average(float *array, int left, int right);

#endif