// #include "ti/driverlib/m0p/dl_core.h"
// #include "ti_msp_dl_config.h"
// #include "oled.h"
// #include "motor.h"

// int main(void)
// {
//     SYSCFG_DL_init();
//     OLED_Init();
//     OLED_ShowString(8,2,(uint8_t *)"HELLOWORLD\0",8);
//     OLED_Clear();
//     OLED_ShowString(16,2,(uint8_t *)"MOLI",8);
//     // 电机初始化
//     motor_init();
//     motor_duty(PWM0_TIM, PWM0_CHANNEL, 100); // 设置电机速度
//     motor_duty(PWM1_TIM, PWM1_CHANNEL, 100);
//     // 增加外部中断代码
//     NVIC_EnableIRQ(ENCODER_INT_IRQN);
//     // 增加定时器中断代码
//     OLED_ShowString(24,2,(uint8_t *)"HELLOWORLD",16);
//     // 清除定时器中断标志 Clear the timer interrupt flag
//     NVIC_ClearPendingIRQ(ENCODER_TIM0_INST_INT_IRQN);
//     NVIC_ClearPendingIRQ(ENCODER_TIM1_INST_INT_IRQN);
//     // 使能定时器中断 Enable timer interrupt
//     NVIC_EnableIRQ(ENCODER_TIM0_INST_INT_IRQN);
//     NVIC_EnableIRQ(ENCODER_TIM1_INST_INT_IRQN);

//     while (1) {
//         OLED_ShowNum(0, 0, motor[LEFT_MOTOR].encoder_count, 5, 16);
//     }
// }

#include "ti_msp_dl_config.h"
 
volatile unsigned int delay_times = 0;
volatile unsigned char uart_data = 0;

void delay_ms(unsigned int ms);
void uart0_send_char(char ch);
void uart0_send_string(char* str);

int main(void)
{
    SYSCFG_DL_init(); 
    //清除串口中断标志 Clear the serial port interrupt flag
    NVIC_ClearPendingIRQ(Bluetooth_INST_INT_IRQN);
    //使能串口中断 Enable serial port interrupt
    NVIC_EnableIRQ(Bluetooth_INST_INT_IRQN);

    while (1)
    { 
        // //LED引脚输出高电平 LED pin outputs high level
        // DL_GPIO_setPins(LED1_PORT, LED1_PIN_2_PIN);
        // delay_ms(500);
        // //LED引脚输出低电平 LED pin outputs low level
        // DL_GPIO_clearPins(LED1_PORT, LED1_PIN_2_PIN);
        // delay_ms(500);
    }
}

// //搭配滴答定时器实现的精确ms延时 Accurate ms delay with tick timer
// void delay_ms(unsigned int ms)
// {
//     delay_times = ms;
//     while( delay_times != 0 );
// }
//串口发送单个字符 Send a single character through the serial port
void uart0_send_char(char ch)
{
    //当串口0忙的时候等待，不忙的时候再发送传进来的字符
    // Wait when serial port 0 is busy, and send the incoming characters when it is not busy
    while( DL_UART_isBusy(Bluetooth_INST) == true );
    //发送单个字符 Send a single character
    DL_UART_Main_transmitData(Bluetooth_INST, ch);
}
//串口发送字符串 Send string via serial port
void uart0_send_string(char* str)
{
    //当前字符串地址不在结尾 并且 字符串首地址不为空
    // The current string address is not at the end and the string first address is not empty
    while(*str!=0&&str!=0)
    {
        //发送字符串首地址中的字符，并且在发送完成之后首地址自增
        // Send the characters in the first address of the string, and increment the first address after sending.
        uart0_send_char(*str++);
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
            uart0_send_char(uart_data);
            break;

        default://其他的串口中断 Other serial port interrupts
            break;
    }
}
