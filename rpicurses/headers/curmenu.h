#ifndef CUR_MENU_H
#define CUR_MENU_H

#include <iostream>
#include "registerMap.h"
#include "arduPi.h"
#include "sim7600.h"
#include <ctime>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "utils.h"
#include <pthread.h>


void handler_stop(int s);
int curmenu(); 
#endif