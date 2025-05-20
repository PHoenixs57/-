#include "bluetooth.h"

// bool dmaBusy;


void Bluetooth_Init(void)
{ 
    DL_SYSCTL_disableSleepOnExit();
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);
    /* Optional delay to ensure UART TX is idle before starting transmission */
    // delay_cycles(UART_TX_DELAY);
    // dmaBusy = false;
}


/*------------------------- 发送函数实现 -------------------------*/
void UART_SendByDMA(const uint8_t *data, uint16_t length)
{
    // 等待前一次传输完成
    //while(dmaBusy); 

    // dmaBusy = true;

    // 配置DMA
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)&data[0]);
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)&(UART_0_INST->TXDATA));
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, length);


    // 使能UART的DMA发送
    DL_UART_enableDMATransmitEvent(UART_0_INST);
    // 启动DMA传输
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

}
 
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

