#include "motor.h"
#include <stdbool.h>
/*反转为前进 即AIN0为0，AIN1为1*/

motor_prams motor[MOTOR_NUM];

void gpio_set(GPIO_Regs* gpio, uint32_t pins, uint8_t value)
{
    value ? DL_GPIO_setPins(gpio, pins) : DL_GPIO_clearPins(gpio, pins);
}

void motor_init(void)
{
    // 初始状态：左右电机均制动
    gpio_set(MOTOR_PORT, AIN0, LOW);
    gpio_set(MOTOR_PORT, AIN1, HIGH);
    gpio_set(MOTOR_PORT, BIN0, HIGH);
    gpio_set(MOTOR_PORT, BIN1, HIGH);
    DL_TimerG_setCaptureCompareValue(PWM0_TIM, 0, PWM0_CHANNEL);
    DL_TimerG_setCaptureCompareValue(PWM1_TIM, 0, PWM1_CHANNEL);
}

// ------------------------------------
// @brif    电机速度控制
// @param   gptimer 选择定时器 
// @param   ccIndex 选择定时器通道
// @param   duty    占空比（0~1000）
// @return  void
// Sample usage: motor_duty(PWM0_TIM, PWM0_CHANNEL, 0)
// ------------------------------------
void motor_duty(GPTIMER_Regs *gptimer, DL_TIMER_CC_INDEX ccIndex, int duty)
{
  if (duty < 0) duty = 0;
  else if (duty >= MAX_DUTY) duty = MAX_DUTY;
  DL_TimerG_setCaptureCompareValue(gptimer, duty, ccIndex);
}
