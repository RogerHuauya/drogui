#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#define max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) > (y) ? (y) : (x))
//#define abs(x)  ( (x > 0) ? (x) : (-1*(x)) )
#define copysign(x,y)  ( ( (x > 0 && y< 0) || (x < 0 && y > 0) ) ? (x) : (-1*(x)))
#define pi acos(-1)

#endif
