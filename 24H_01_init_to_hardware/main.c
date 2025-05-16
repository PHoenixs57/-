#include "ti_msp_dl_config.h"
#include "oled.h"
#include "motor.h"

int main(void)
{
    SYSCFG_DL_init();
    OLED_Init();
    OLED_ShowString(8,2,(uint8_t *)"ZHONGJINGYUAN",16);
    // 增加外部中断代码
    NVIC_EnableIRQ(ENCODER_INT_IRQN);
    // 增加定时器中断代码
    //清除定时器中断标志 Clear the timer interrupt flag
    NVIC_ClearPendingIRQ(ENCODER_TIM0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(ENCODER_TIM1_INST_INT_IRQN);
    //使能定时器中断 Enable timer interrupt
    NVIC_EnableIRQ(ENCODER_TIM0_INST_INT_IRQN);
    NVIC_EnableIRQ(ENCODER_TIM1_INST_INT_IRQN);

    while (1) {
    }
}

