#include "pid.h"
#include "ti_msp_dl_config.h"
#include "motor.h"
#include "bluetooth.h"
#include "delay.h"
#include "pid.h"

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
                    uart0_send_char(motor[LEFT_MOTOR].encoder_count);
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

//定时器的中断服务函数 已配置为10毫秒的周期
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

//串口的中断服务函数 Serial port interrupt service function
void Bluetooth_INST_IRQHandler(void)
{
    //如果产生了串口中断 If a serial port interrupt occurs
    switch( DL_UART_getPendingInterrupt(Bluetooth_INST) )
    {
        case DL_UART_IIDX_RX://如果是接收中断 If it is a receive interrupt
            //接发送过来的数据保存在变量中 The data sent is saved in the variable
            uart_data = DL_UART_Main_receiveData(Bluetooth_INST);
            //将保存的数据再发送出去 Send the saved data again
            uart0_send_string("hello\r\n");
            if (uart_data == 'o') {
                DL_GPIO_setPins(LED1_PORT, LED1_PIN_2_PIN);
                motorA.target += 5;
            }
            else if(uart_data == 'f') {
                DL_GPIO_clearPins(LED1_PORT, LED1_PIN_2_PIN);
                motorA.target -= 5;
            }
            break;

        default://其他的串口中断 Other serial port interrupts
            break;
    }
}


//滴答定时器的中断服务函数 Tick ??timer interrupt service function
void SysTick_Handler(void)
{
    if( delay_times != 0 )
    {
        delay_times--;
    }
}

