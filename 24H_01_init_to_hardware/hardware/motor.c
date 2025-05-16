#include "motor.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include <cstdint>
#include <stdbool.h>
#define MAX_DUTY 1000

motor_prams left_motor, right_motor;

void gpio_set(GPIO_Regs* gpio, uint32_t pins, uint8_t value)
{
    value ? DL_GPIO_setPins(gpio, pins) : DL_GPIO_clearPins(gpio, pins);
}

void motor_init(void)
{
    DL_TimerG_setCaptureCompareValue(PWM_0_INST, 0, GPIO_PWM_0_C0_IDX);
    DL_TimerG_setCaptureCompareValue(PWM_1_INST, 0, GPIO_PWM_1_C1_IDX);
}

// ------------------------------------
// @brif    电机速度控制
// @param   gptimer 选择定时器 
// @param   ccIndex 选择定时器通道
// @param   duty    占空比（0~1000）
// @return  void
// Sample usage: motor_duty(PWM_0_INST, GPIO_PWM_0_C0_IDX, 0)
// ------------------------------------
void motor_duty(GPTIMER_Regs *gptimer, DL_TIMER_CC_INDEX ccIndex, int duty)
{
  if (duty < 0) duty = 0;
  else if (duty >= MAX_DUTY) duty = MAX_DUTY;
  DL_TimerG_setCaptureCompareValue(gptimer, duty, ccIndex);
  gpio_set(GPIOA_PORT, GPIOA_AIN0_PIN, motor_dir);
  gpio_set(GPIOA_PORT, GPIOA_AIN1_PIN, !motor_dir);
}
