#include "sensors.hpp"

void initSensor(sensor *s, double delay, double noise, double bias, double freq, double* var){
	(s->tim) = (s->head) = 0;
	(s->delay) = delay;
	(s->noise) = noise;
	(s->freq) = freq;
	(s->bias) = bias;
	(s->delay) = (int) (delay*freq);
	(s->var) = var;
	(s->tim_ant) = 0;
	for(int i = 0 ; i < BUFF_SIZE; i++) (s->mem[i]) = 0;
}

double readSensor(sensor *s, double tim){
		
	if(tim - (s->tim_ant) + 1e-6>= 1/((s->freq))){
		//printf("%f %d\n", tim, s->delay);

		double act = *(s->var);
		act = act +  (s->noise)*(rand()%2000-1000)/1000.0;

		s->mem[(s->head)++] = act;
		(s->head) = (s->head) % BUFF_SIZE;

		(s->tim_ant) = tim;
	}

	return s->mem[((s->head) - 1 - (s->delay) + BUFF_SIZE) % BUFF_SIZE] + s->bias;
}
