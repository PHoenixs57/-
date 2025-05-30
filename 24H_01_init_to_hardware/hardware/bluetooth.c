#include "bluetooth.h"

// bool dmaBusy;

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

// void Bluetooth_Init(void)
// { 
//     DL_SYSCTL_disableSleepOnExit();
//     NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
//     DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);
//     /* Optional delay to ensure UART TX is idle before starting transmission */
//     // delay_cycles(UART_TX_DELAY);
//     // dmaBusy = false;
// }


/*------------------------- 发送函数实现 -------------------------*/
// void UART_SendByDMA(const uint8_t *data, uint16_t length)
// {
//     // 等待前一次传输完成
//     //while(dmaBusy); 

//     // dmaBusy = true;

//     DL_SYSCTL_disableSleepOnExit();
//     NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
//     // 配置DMA
//     DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)&data[0]);
//     DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)&(UART_0_INST->TXDATA));
//     DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, length);


//     // 使能UART的DMA发送
//     DL_UART_enableDMATransmitEvent(UART_0_INST);
//     // 启动DMA传输
//     DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

// }
 
// void UART_0_INST_IRQHandler(void)
// {
//     switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
//         case DL_UART_MAIN_IIDX_EOT_DONE:
//         case DL_UART_MAIN_IIDX_DMA_DONE_TX:
//             dmaBusy = false;
//             // uartDmaStatus.lastTxComplete = true;
//             DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
//             break;
//         default:
//             break;
//     }
// }