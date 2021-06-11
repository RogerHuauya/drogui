#ifndef SCURVE_H
#define SCURVE_H
#include "_main.h"


/**
 * @brief Structure Scurve  
 * @param tpacket  TeraRanger packet used for comunnication (tRPacket*)
 * @param timeout  Maximum time expected to get data from sensor (uint32_t)
*/
typedef struct _scurve{
    
    float ini, fin, period;
    uint32_t ini_time;

} scurve;

/**
 * @brief Convert time domain to s domain 
 * @param sc (Pointer of structure scurve)
 * @param time Current time (float)
 * @return S value
*/
float t2s(scurve *sc, float time);

/**
 * @brief Initialize S trayectory
 * @param sc (Pointer of structure scurve)
 * @param ini Initial time (float)
 * @param fin Final time (float) 
 * @param period Sample time (float)  
 * @param ini_time Current time (uint32_t)
*/
void setTrayectory(scurve *sc, float ini, float fin, float period, uint32_t ini_time);

/**
 * @brief Get point of s-trayectory 
 * @param sc  (Pointer of structure scurve)
 * @param time Current time (uint32_t)
 * @return Point of s-domain
*/
float getSetpoint(scurve *sc, uint32_t time);



#endif