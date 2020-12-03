#ifndef THREADED_H
#define THREADED_H
#include <iostream>
#include "registerMap.h"
#include "arduPi.h"
#include "sim7600.h"
#include <ctime>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <unistd.h> 

#include "utils.h"
#include <pthread.h>
void *logging(void *threadid);
void *gps_data(void *threadid);

#endif