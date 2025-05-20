#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "ti_msp_dl_config.h"


// volatile bool gCheckUART, gDMADone;
void UART_SendByDMA(const uint8_t *data, uint16_t length);
void Bluetooth_Init(void);

#endif
