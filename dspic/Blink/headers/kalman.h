#ifndef KALMAN_H
#define KALMAN_H

#include "utils.h"
#include "matlib.h"
#include "serial.h"
#include "i2c.h"
#include "registerMap.h"

void kalmanUpdate();
extern mat p, v, Rq, u, bias_u;

#endif