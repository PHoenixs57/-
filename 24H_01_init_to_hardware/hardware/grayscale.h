#ifndef __GRAYSCALE_H
#define __GRAYSCALE_H

#include "ti_msp_dl_config.h"
#include "oled.h"
#define  RX_BUFFER_SIZE 43
void grayscale_init();
void grayscale_start(char *str,int* rx_count,bool* dataReceived);
void grayscale_stop();
void uart0_send_char(char ch);
void uart0_send_string(char* str);

#endif  