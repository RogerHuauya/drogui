#ifndef KALMAN_H
#define KALMAN_H

#include "macros.h"

#ifdef KALMANPV
	#include "kalmanPV.h"
#elif defined KALMANP
	#include "kalmanP.h"
#endif


#endif