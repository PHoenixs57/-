#include "ti_msp_dl_config.h"
#include "oled.h"
#include "grayscale.h"
volatile unsigned char uart_data = 0;
char grayscale_start_str[]="$0,0,1#";
volatile unsigned char grayscale_uart_receive;//8位
volatile char rxBuffer[RX_BUFFER_SIZE]; // 接收缓冲区
int rx_count;
volatile bool dataReceived = false; 

int main(void)
{
    SYSCFG_DL_init();
    // OLED_Init();
    // OLED_ShowString(8,2,(uint8_t *)"ZHONGJINGYUAN",16);
    // grayscale_init();
    
    
    grayscale_start(grayscale_start_str,&rx_count,&dataReceived);
    //OLED_Clear();
    while (1) {
        __WFI();
    }
}
void UART_0_INST_IRQHandler(void)
{
    //OLED_Clear();
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            rxBuffer[rx_count++] = DL_UART_Main_receiveData(UART_0_INST);
            uart0_send_char(rxBuffer[rx_count-1]);
            if(rx_count>=RX_BUFFER_SIZE)
            {
                if(rxBuffer[0]=='$'&&rxBuffer[RX_BUFFER_SIZE-1]=='#')
                {
                    //OLED_ShowString(8,3,(uint8_t*)rxBuffer, 16);
                    grayscale_uart_receive  = 0;
                    for(char i=0;i<8;i++)
                    {
                        if(rxBuffer[6+i*5] == '1')
                        grayscale_uart_receive |=(0x80>>i); 
                    }
                    dataReceived = true;
                }
                //开启下一轮接收
               rx_count = 0;
            }
            break;
        default:
            break;
    }
}

