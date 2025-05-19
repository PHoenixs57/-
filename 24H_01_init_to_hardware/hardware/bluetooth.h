#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include "ti_msp_dl_config.h"


void Bluetooth_Init();
void uart0_send_char(char ch);
void uart0_send_string(char* str);


#endif
