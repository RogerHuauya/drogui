#ifndef PWM_H
#define PWM_H
#include "tim.h"

/**
 * @brief Structure PWM 
 * @param Timer
 * @param Channel
 * @param DutyCycle 
*/
typedef struct _pwm{
    TIM_HandleTypeDef* timer;
    uint32_t channel;
    volatile uint32_t *duty;
}pwm;

/**
 * @brief Initialize PWM with a specific Timer and DutyCycle
 * @param p PWM (struc _pwm)
 * @param timer Timer to be used (TIM_HandleTypeDef*)
 * @param channel Timer Channel (uint32_t)
 * @param duty Oneshot125 (uint32_t)
*/
void initPwm(pwm* p, TIM_HandleTypeDef* timer, uint32_t channel, volatile uint32_t *duty);
/**
 * @brief Set a specific percentage of the Oneshot125 
 * @param p PWM (struc _pwm)
 * @param percent (Oneshot125 percentage)
*/
void setPwm(pwm* p, float percent);

#endif