#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "ti_msp_dl_config.h"
#include "ti/driverlib/m0p/dl_core.h"

void uart0_send_char(char ch);
void uart0_send_string(char* str);

extern uint8_t uart_data;

#endif
