#include "pwm.h"


void initPwm(pwm* p, TIM_HandleTypeDef* timer, uint32_t channel, volatile uint32_t *duty){
    p->timer = timer;
    p->channel = channel;
    p->duty = duty;
}