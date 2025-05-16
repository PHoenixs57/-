#ifndef __MOTOR_H
#define __MOTOR_H
#include "motor.h"
#include "ti_msp_dl_config.h"
#include <cstdint>

//adding ur own compiler controlling pragmas
// ------------------------
// @brif  电机参数表示
// @pram  motor_dir     电机转动方向，1为正转，0为反转
// @pram speed_now      电机转速 10ms内编码器计数量
// @pram encoder_count  编码器计数量
// ------------------------
typedef struct
{
    uint8_t motor_dir = 1;
    int speed_now = 0;
    int encoder_count = 0;
} motor_prams;

void motor_init(void);
void motor_duty(GPTIMER_Regs *gptimer, DL_TIMER_CC_INDEX ccIndex, int duty);

extern motor_prams left_motor, right_motor;
#endif
