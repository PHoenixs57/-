#include "main.h"

//灰度传感器DMA串口接收中断
void grayscale_INST_IRQHandler(void)
{
    uint8_t uart_data = 0x00; 
    bool gCheck = false;
    switch (DL_UART_Main_getPendingInterrupt(grayscale_INST)) {
        case DL_UART_MAIN_IIDX_DMA_DONE_RX:
        //$D,x1:0,x2:0,x3:0,x4:0,x5:0,x6:0,x7:0,x8:0#
        //X1亮灯（在黑线上）其它探头灯不亮 B01111111
        //GrayScale_Send_String(gRxGraycScalePacket);
        //UART_SendByDMA(gRxGraycScalePacket,UART_GrayScale_PACKET_SIZE);
        if((gRxGraycScalePacket[0]=='$')&&(gRxGraycScalePacket[UART_GrayScale_PACKET_SIZE-1]=='#'))
            {
                for(uint8_t i=0;i<8;i++)
                {
                    if(gRxGraycScalePacket[6+i*5]=='1')
                    {
                        uart_data |= (0x80>>i);
                    }
                }
                //GrayScale_Send_Char(uart_data);
                //uart0_send_char(uart_data);
                gCheck = true;
            }
            wGrayScale_Data(uart_data,gCheck);
            break;
        default:
            break;
    }
}

void Gyroscpe_INST_IRQHandler(void)
{
    int i;
    // uart0_send_char('A');
    switch (DL_UART_Main_getPendingInterrupt(Gyroscpe_INST)) {
        case DL_UART_MAIN_IIDX_DMA_DONE_RX:
            //wt61_rx_buffer
            
            uart0_send_char('A');
            delay_ms(500);
            if(strlen(wt61_rx_buffer)==0) uart0_send_string("0");
            else if (strlen(wt61_rx_buffer)==44) uart0_send_string("44");
            uart0_send_string(wt61_rx_buffer);
            // for(i=0;i<11;i++)
            // {
            //     if((wt61_rx_buffer[i] == WT61_FRAME_HEADER)&&(wt61_rx_buffer[i] ==WT61_TYPE_ACCEL))
            //     {
            //         store_wt61_data(&wt61_rx_buffer[i]);
            //         Gyroscpe_Stop();
            //         break;
            //     }
            // }
            // if(i==11)
            // uart0_send_string("rx_err\n");

            
            // if (wt61_rx_buffer[0] == WT61_FRAME_HEADER) 
            // {
            //     if( wt61_rx_buffer[1] == WT61_TYPE_ACCEL) 
            //     {
            //         store_ACCEL_data();
            //     }
            //     else if(wt61_rx_buffer[1] == WT61_TYPE_GYRO)
            //     {
            //         store_GYRO_data();
            //     }
            //     else if(wt61_rx_buffer[1] == WT61_TYPE_ANGLE)
            //     {
            //         store_ANGLE_data();
            //     }
            // }
            break;
        default:
            break;
    }
}
// void SysTick_Handler(void)
// {
//         if( delay_times != 0 )
//         {
//                 delay_times--;
//         }
// }
//蓝牙调试串口的中断服务函数 Serial port interrupt service function
// void Bluetooth_INST_IRQHandler(void)
// {
//     volatile unsigned char uart_data = 0;
//     //如果产生了串口中断 If a serial port interrupt occurs
//     switch( DL_UART_getPendingInterrupt(Bluetooth_INST) )
//     {
//         case DL_UART_IIDX_RX://如果是接收中断 If it is a receive interrupt
//             //接发送过来的数据保存在变量中 The data sent is saved in the variable
//             uart_data = DL_UART_Main_receiveData(Bluetooth_INST);
//             //将保存的数据再发送出去 Send the saved data again
//             uart0_send_char(uart_data);
//             break;

//         default://其他的串口中断 Other serial port interrupts
//             break;
//     }
// }

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