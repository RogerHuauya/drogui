#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"



#define EPS 1e-9
#define pi acos(-1)
#define copysign(x, y) ( ( (((x) > 0) && ((y) < 0)) || (((x) < 0) && ((y) > 0)) ) ? (-1)*(x) : (x) )


enum STATES {DESCEND, SEC_STOP, WAITING, ARM_MOTORS, CONTROL_LOOP};

extern uint8_t i2cReg[300][4];
extern bool security;
extern int state;
extern int calib_status;


extern double distance;


/**
 * @brief Convert bytes to int32
 * @param bytesint32 (uint8_t *) Value
 * @return int32 value 
 */
int32_t bytestoint32(uint8_t *bytesint32);

/**
 * @brief Convert int32 to bytes
 * @param n Integer value (int32_t)
 * @param bytesint32 Value (uint8_t *) 
 * @return  
 */
void int32tobytes(int32_t n,uint8_t *bytesint32);
/**
 * @brief Convert float to bytes
 * @param n Floating value (float)
 * @param bytesfloat Value (uint8_t *) 
 * @return  
 */
void floattobytes(float n, uint8_t* bytesfloat);

/**
 * @brief Convert bytes to float
 * @param bytesfloat  Bytes Array (uint8_t *)
 * @return Floating value 
 */
float bytestofloat(uint8_t *bytesfloat);

/**
 * @brief Get value from register
 * @param reg Register (uint8_t) 
 * @return Value (float)
 */
float getReg(uint8_t reg);

/**
*   @brief Set a register with a floating value
*   @param reg Register (uint8_t)
*   @param val Value (float)
*   @return 
*/
void setReg(uint8_t reg, float val);
/**
*   @brief Get the Euler's angles from the quaternion 
*   @param q0 Quaternion Component (float)
*   @param q1 Quaternion Component (float)
*   @param q2 Quaternion Component (float)
*   @param q3 Quaternion Component (float)
*   @param roll Roll (float*)
*   @param pitch Pitch (float*)
*   @param yaw Yaw (float*)
*   @return 
*/
void getEuler(float q0, float q1, float q2, float q3, float* roll, float *pitch, float* yaw);
/**
*   @brief Difference between two angles  
*   @param angle1 (float)
*   @param angle2 (float)
*   @return The difference
*/
float angle_dif(float angle1, float angle2);

//!  @brief  Low Pass Filter with K array (6 elements), V array(7 elements) and Fc/Fs = 1/10
    extern float k_1_10[];
    extern float v_1_10[];
//@}

//!  @brief  Low Pass Filter with K array (5 elements), V array(6 elements) and Fc/Fs = 3/10
    extern float k_3_10[];
    extern float v_3_10[];
//@}

//!  @brief  Low Pass Filter with K array (4 elements), V array(5 elements) and Fc/Fs = 5/100
    extern float k_1_20[];
    extern float v_1_20[];
//@}

//!  @brief  Low Pass Filter with K array (2 elements), V array(3 elements) and Fc/Fs = 4/1000
    extern float k_1_250[];
    extern float v_1_250[];
//@}

//!  @brief  Low Pass Filter with K array (3 elements), V array(4 elements) and Fc/Fs = 10/1000
    extern float k_1_100[];
    extern float v_1_100[];
//@}

//!  @brief  Low Pass Filter with K array (3 elements), V array(4 elements) and Fc/Fs = 20/1000
    extern float k_1_50[];
    extern float v_1_50[];
//@}

//!  @brief  Low Pass Filter with K array (3 elements), V array(4 elements) and Fc/Fs = 15/1000
    extern float k_3_200[];
    extern float v_3_200[];
//@}

//!  @brief  High Pass Filter with K array (3 elements), V array(4 elements) and Fc/Fs = 15/1000
    extern float hk_1_500[];
    extern float hv_1_500[];
//@}

//!  @brief  High Pass Filter with K array (3 elements), V array(4 elements) and Fc/Fs = 15/1000
    extern float hk_1_200[];
    extern float hv_1_200[];
//@}


extern float SG7[];
extern float SG10[];
extern float SG13[];

/**
*   @brief Increse a variable with little steps until a desired value   
*   @param step (float)
*   @param desired Desired value (float)
*   @param var Varible to be increased (float *)
*   @return 
*/
void rampValue(float *var, float desired, float step);


void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data);
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data);

float dis3d(float x,float y,float z, float a, float b, float c);

#endif
