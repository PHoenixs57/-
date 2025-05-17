#include "ti_msp_dl_config.h"
#include "motor.h"

//Group1的中断服务函数 Interrupt service function of Group1
void GROUP1_IRQHandler(void)
{
    //读取Group1的中断寄存器并清除中断标志位
    // Read the interrupt register of Group1 and clear the interrupt flag
    switch( DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1) )
    {
        //检查是否是KEY的GPIOA端口中断，注意是INT_IIDX，不是PIN_18_IIDX
        // Check if it is the GPIOA port interrupt of KEY, note that it is INT_IIDX, not PIN_18_IIDX
        case ENCODER_INT_IIDX:
            // 如果E0A下降，则触发中断
            if(DL_GPIO_readPins(ENCODER_PORT, ENCODER_E0A_PIN))
            {
                // 检查E0B是低电平还是高电平, 低电平正转，高电平反转
                if (DL_GPIO_readPins(ENCODER_PORT, ENCODER_E0B_PIN))
                {
                    motor[LEFT_MOTOR].encoder_count++;
                }
                else {
                    motor[LEFT_MOTOR].encoder_count--;
                }
            }
            // 如果E1A下降，则触发中断
            if(DL_GPIO_readPins(ENCODER_PORT, ENCODER_E1A_PIN))
            {
                // 检查E1B是低电平还是高电平, 低电平正转，高电平反转
                if (DL_GPIO_readPins(ENCODER_PORT, ENCODER_E1B_PIN))
                {
                    motor[RIGHT_MOTOR].encoder_count++;
                }
                else {
                    motor[RIGHT_MOTOR].encoder_count--;
                }
            }
        break;
    }
}

//定时器的中断服务函数 已配置为1秒的周期
//The timer interrupt service function has been configured to a period of 1 second
void TIMER_0_INST_IRQHandler(void)
{
    //如果产生了定时器中断 If a timer interrupt occurs
    switch( DL_TimerG_getPendingInterrupt(ENCODER_TIM0_INST) )
    {
        case DL_TIMER_IIDX_ZERO://如果是0溢出中断 If it is 0 overflow interrupt
            // 进行速度计算
            motor[LEFT_MOTOR].speed_now = motor[LEFT_MOTOR].encoder_count;
            motor[LEFT_MOTOR].encoder_count = 0;
            break;

        default://其他的定时器中断 Other timer interrupts
            break;
    }
}
//定时器的中断服务函数 已配置为1秒的周期
//The timer interrupt service function has been configured to a period of 1 second
void TIMER_1_INST_IRQHandler(void)
{
    //如果产生了定时器中断 If a timer interrupt occurs
    switch( DL_TimerG_getPendingInterrupt(ENCODER_TIM1_INST) )
    {
        case DL_TIMER_IIDX_ZERO://如果是0溢出中断 If it is 0 overflow interrupt
            //进行速度计算
            motor[RIGHT_MOTOR].speed_now = motor[RIGHT_MOTOR].encoder_count;
            motor[RIGHT_MOTOR].encoder_count = 0;
            break;

        default://其他的定时器中断 Other timer interrupts
            break;
    }
}