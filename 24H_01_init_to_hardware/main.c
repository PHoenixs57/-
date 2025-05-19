#include "main.h"


// uart0_send_string(char* str)蓝牙调试
char Bluetooth_BUF[] = "Hello world\r\n";
int count = 0;
int main(void)
{
    SYSCFG_DL_init(); 
    Bluetooth_Init();
    uart0_send_string(Bluetooth_BUF);
    // GrayScale_Init();
    // GrayScale_Start();
    Gyroscpe_Init();
    uart0_send_string("1\r\n");
    Gyroscpe_Start();
    uart0_send_string("2\r\n");
    // char tx_buffer[128];

    while (1){
        
        // if(Gyroscpe_Data_Ready())
        // {
        //     snprintf((char*)tx_buffer, sizeof(tx_buffer),
        //     "Accel: X=%.2f Y=%.2f Z=%.2f\n"
        //     "Gyro : X=%.2f Y=%.2f Z=%.2f\n"
        //     "angle: X=%.2f Y=%.2f Z=%.2f\n", 
        //     imu_data.accel[0], imu_data.accel[1], imu_data.accel[2],
        //     imu_data.gyro[0], imu_data.gyro[1], imu_data.gyro[2],
        //     imu_data.angle[0], imu_data.angle[1], imu_data.angle[2]);
            
           
        //     uart0_send_string(tx_buffer);
        // }
    }
}
