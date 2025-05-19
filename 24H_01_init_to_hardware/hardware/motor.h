#ifndef __MOTOR_H
#define __MOTOR_H
#include "ti_msp_dl_config.h"
#include "ti/driverlib/m0p/dl_core.h"
//adding ur own compiler controlling pragmas
#define MAX_DUTY 50000
// 电机最大数量
#define MOTOR_NUM          2

#define LOW                0
#define HIGH               1
// 电机控制GPIO端口/引脚
#define MOTOR_PORT         GPIOA              
#define AIN0               GPIOA_AIN0_PIN     
#define AIN1               GPIOA_AIN1_PIN     
#define BIN0               GPIOA_BIN0_PIN     
#define BIN1               GPIOA_BIN1_PIN     
// 电机PWM输出定时器/通道
#define PWM0_TIM           PWM_0_INST         
#define PWM0_CHANNEL       GPIO_PWM_0_C0_IDX  
#define PWM1_TIM           PWM_1_INST         
#define PWM1_CHANNEL       GPIO_PWM_1_C1_IDX  

typedef enum 
{
    LEFT_MOTOR = 0,
    RIGHT_MOTOR
} Motor_ID;

// ------------------------
// @brif  电机参数表示
// @pram  motor_dir     电机转动方向，1为正转，0为反转
// @pram speed_now      电机转速 10ms内编码器计数量
// @pram encoder_count  编码器计数量
// ------------------------
typedef struct
{
    uint8_t motor_dir;
    int speed_now;
    int encoder_count;
} motor_prams;

void motor_init(void);
void motor_duty(GPTIMER_Regs *gptimer, DL_TIMER_CC_INDEX ccIndex, int duty);

extern motor_prams motor[MOTOR_NUM];
#endif
