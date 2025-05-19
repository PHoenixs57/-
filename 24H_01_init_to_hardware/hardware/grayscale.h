#ifndef __GRAYSCALE_H
#define __GRAYSCALE_H
#include "ti_msp_dl_config.h"

#define UART_GrayScale_PACKET_SIZE (43)

void wGrayScale_Data(uint8_t uart_data,bool gCheck);
uint8_t rGrayScale_Data();
void GrayScale_Init();
void GrayScale_Start();
void GrayScale_Stop();
void GrayScale_Send_Char(char ch);
void GrayScale_Send_String(char* str);

extern volatile uint8_t gRxGraycScalePacket[UART_GrayScale_PACKET_SIZE];
#endif  