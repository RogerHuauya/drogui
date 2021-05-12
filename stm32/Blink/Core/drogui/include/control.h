#ifndef CONTROL_H
#define CONTROL_H

#include "utils.h"
#include "filter.h"


enum TYPES_PID {NORMAL = 0, P2ID = 1, INDEXED = 2, PIDABS = 4, D_FILTER = 8, D_INT = 16, D_SG = 32};
/**
 * @brief Struct PID
 * @param Kp Kp Array (float)
 * @param Ki Ki Array (float)
 * @param Kd Kd Array (float)
 * @param erri  Integral component (float)
 * @param err_ant1 Previous error (float)
 * @param err_ant2 Previous error (float) 
 * @param errd Derivative of error (float)
 * @param u Control law (float)
 * @param tant Previous time (uint32_t)
 * @param isat Integral component saturation (float)
 * @param osat Output saturation (float)
 * @param type PID Type NORMAL-P2ID-INDEXED-PIDABS-D_FILTER
 * @param N_filt  Coefficient of derivative component 
*/
typedef struct _pid{
    float kp[5], kd[5], ki[5];
    float threshold[5];
    float erri ,err_ant2, err_ant1;
    float errd,errd_ant,errd_acum;
    float u, u_ant;
    uint32_t tant;
    float isat, osat, dt;
    int type;
    filter f;
    float N_filt;
    savGolDFilter sgd;
} pid;
/**
 * @brief Get control law from PID controller from the error between Reference and Current Value 
 * @param p PID (pointer to struct pid)
 * @param error Current Error (float)
 * @param t Current time (uint32_t)
 * @param h Compensation (float)
 * @return u Control law
*/
float computePid(pid* p, float error, uint32_t t, float h);
float computePidD(pid* p, float error, uint32_t t, float h, float dp);
/**
 * @brief Reset PID controller, the error as well as the integral and derivative components become zeros 
 * @param p PID (pointer to struct pid) 
 * @param ti Time (float)
 * @return u Value of Control law
*/
void resetPid(pid* p, uint32_t ti);
/**
 * @brief Initialize PID Structure
 * @param p PID (struct pid)
 * @param kp Proportional constant (float)
 * @param ki Integral constant (float)
 * @param kd Derivative constant (float)
 * @param ti Time (uint32_t)
 * @param N Low pass Frecuency of derivative component
 * @param isat Integral component saturation (float)
 * @param osat Output saturation (float)
 * @param tipo PID Type NORMAL-P2ID-INDEXED-PIDABS-D_FILTER
*/
void initPid(pid* p, float kp, float kd, float ki,uint32_t ti, float N, float isat,float osat, int tipo );
/**
 * @brief Initialize PID-(Derivative term Filter) Structure
 * @param p PID (pointer to struct pid)
 * @param kp Proportional constant (float)
 * @param ki Integral constant (float)
 * @param kd Derivative constant (float)
 * @param ti Time (uint32_t)
 * @param N Low pass Frecuency of derivative component
 * @param isat Integral component saturation (float)
 * @param osat Output saturation (float)
 * @param tipo PID Type NORMAL-P2ID-INDEXED-PIDABS-D_FILTER
 * @param a Denominator elements (float*)
 * @param b Numerator elements (float*)
*/
void initPidFilter(pid* p, float kp, float kd, float ki,uint32_t ti, float N, float isat,float osat,int tipo, int n, float*a, float*b);

#endif
