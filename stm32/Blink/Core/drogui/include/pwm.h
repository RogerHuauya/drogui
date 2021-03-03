#ifndef PWM_H
#define PWM_H
#include "tim.h"

typedef struct _pwm{
    TIM_HandleTypeDef* timer;
    uint32_t channel;
    volatile uint32_t *duty;
}pwm;

void initPwm(pwm* p, TIM_HandleTypeDef* timer, uint32_t channel, volatile uint32_t *duty);
void setPwm(pwm* p, float percent);

#endif