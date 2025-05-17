#include "grayscale.h"


void grayscale_init()
{
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

void grayscale_start(char *str,int* rx_count,bool* dataReceived)
{
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);   
    uart0_send_string(str);
    *rx_count = 0;
    *dataReceived = false;
}

void grayscale_stop()
{
    NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
}
// unsigned char grayscale_receive(bool dataReceived)
// {
//     if(dataReceived==true)
//     {
//         return grayscale_uart_receive;
//     }
//     return 0;
// }
//串口发送单个字符 Send a single character through the serial port
void uart0_send_char(char ch)
{
    //当串口0忙的时候等待，不忙的时候再发送传进来的字符
    // Wait when serial port 0 is busy, and send the incoming characters when it is not busy
    while( DL_UART_isBusy(UART_0_INST) == true );
    //发送单个字符 Send a single character
    DL_UART_Main_transmitData(UART_0_INST, ch);
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

