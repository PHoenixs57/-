#include "grayscale.h"



volatile uint8_t gRxGraycScalePacket[UART_GrayScale_PACKET_SIZE];
volatile uint8_t grayscale_data = 0x00;
volatile bool gCheckGrayScale;
char grayscale_strat_str[] = "$0,0,1#";

void GrayScale_Init()
{
    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&grayscale_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &gRxGraycScalePacket[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, UART_GrayScale_PACKET_SIZE);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
    // /* Confirm DMA channel is enabled */
    while (false == DL_DMA_isChannelEnabled(DMA, DMA_CH0_CHAN_ID)) {
        __BKPT(0);
    }
}

void GrayScale_Start()
{
    NVIC_EnableIRQ(grayscale_INST_INT_IRQN);
    GrayScale_Send_String(grayscale_strat_str);
    gCheckGrayScale = false;
}

void GrayScale_Stop()
{
    NVIC_DisableIRQ(grayscale_INST_INT_IRQN);
}

void wGrayScale_Data(uint8_t uart_data,bool gCheck)
{
    gCheckGrayScale = gCheck;
    grayscale_data = uart_data;
}

uint8_t rGrayScale_Data()
{
    if(gCheckGrayScale == true)
    {
        return grayscale_data;
    }
    else return 0x00;
}
//串口发送单个字符 Send a single character through the serial port
void GrayScale_Send_Char(char ch)
{
    //当串口0忙的时候等待，不忙的时候再发送传进来的字符
    // Wait when serial port 0 is busy, and send the incoming characters when it is not busy
    while( DL_UART_isBusy(grayscale_INST) == true );
    //发送单个字符 Send a single character
    DL_UART_Main_transmitData(grayscale_INST, ch);
}
//串口发送字符串 Send string via serial port
void GrayScale_Send_String(char* str)
{
    //当前字符串地址不在结尾 并且 字符串首地址不为空
    // The current string address is not at the end and the string first address is not empty
    while(*str!=0&&str!=0)
    {
        //发送字符串首地址中的字符，并且在发送完成之后首地址自增
        // Send the characters in the first address of the string, and increment the first address after sending.
        GrayScale_Send_Char(*str++);
    }
}

