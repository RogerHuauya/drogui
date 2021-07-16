#ifndef CONTROL_TASKS_H
#define CONTROL_TASKS_H

#include "control.h"
#include "sensorsTasks.h"
#include "utils.h"
#include "registerMap.h"


extern float M1,M2,M3,M4;

extern float wroll_ref, wpitch_ref, wyaw_ref, roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref, xp_ref, yp_ref;
extern pid wroll_control, wpitch_control, wyaw_control, xp_control, yp_control;
extern float X_C,Y_C;

/**
 * @brief Initialize Control Task
*/
void initControlTasks();
/**
 * @brief Update PID constants
*/
void updatePID();

#endif
