#ifndef SENSORS_HPP
#define SENSORS_HPP

#define BUFF_SIZE 50

#include <bits/stdc++.h>
#include <time.h>

typedef struct _sensor{
	double noise;
	int delay;
	double freq;
	double mem[BUFF_SIZE];
	int head;
	double *var;
	double tim, tim_ant;
	double bias;
} sensor;


void initSensor(sensor *s, double delay, double noise, double bias, double freq, double* var);
double readSensor(sensor *s, double tim);


#endif
