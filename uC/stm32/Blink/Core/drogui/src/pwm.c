#include "pwm.h"




void setPwm(pwm* p, float percent){
    *(p->duty) = 12500.0 + 12500.0*(percent)/100.0; 
}

void initPwm(pwm* p, TIM_HandleTypeDef* timer, uint32_t channel, volatile uint32_t *duty){
    p->timer = timer;
    p->channel = channel;
    p->duty = duty;
    HAL_TIM_PWM_Start(timer, channel);
    setPwm(p, 0);
}