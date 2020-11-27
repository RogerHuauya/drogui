#include "registerMap.h"
#include "sim7600.h"
#include <iostream>
#include "utils.h"

extern rasp_I2C rasp_i2c;
extern bool inputReceived, logging_state, cin_thread;
extern int id_choosen, value, id_threads, id_threads_log;
void send_PID_ROLL();
void send_PID_PITCH();    
void send_PID_YAW();
void send_H();
void send_setpoint();
