#ifndef KALMAN_H
#define KALMAN_H

#include "utils.h"
#include "matlib.h"
#include "serial.h"
#include "i2c.h"
#include "registerMap.h"

void kynematics();
extern mat p, v, Rq, u;

#endif