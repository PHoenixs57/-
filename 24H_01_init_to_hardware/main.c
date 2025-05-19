#include "main.h"


// uart0_send_string(char* str)蓝牙调试
char Bluetooth_BUF[] = "Hello world\r\n";
int count = 0;
int main(void)
{
    SYSCFG_DL_init(); 
    Bluetooth_Init();
    uart0_send_string(Bluetooth_BUF);
    GrayScale_Init();
    GrayScale_Start();
    while (1){}
}
