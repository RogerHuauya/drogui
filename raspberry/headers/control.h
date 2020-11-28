#include "registerMap.h"
#include "sim7600.h"
#include <iostream>
#include "utils.h"

extern rasp_I2C rasp_i2c;
extern bool inputReceived, logging_state, cin_thread;
extern int id_choosen, value, id_thread_menu, id_thread_log, id_thread_gps;
void send_PID();
void send_ref();
void send_setpoint();
void send_comp_mg();
