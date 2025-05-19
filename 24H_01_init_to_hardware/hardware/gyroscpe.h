#ifndef __GYROSCPE_H
#define __GYROSCPE_H
#include "ti_msp_dl_config.h"

// WT61协议相关定义
#define WT61_FRAME_HEADER 0x55
#define WT61_FRAME_LENGTH 11
#define WT61_TYPE_ACCEL   0x51
#define WT61_TYPE_GYRO    0x52
#define WT61_TYPE_ANGLE   0x53

//陀螺仪包大小
#define UART_Gyroscpe_PACKET_SIZE (11)


// 传感器数据结构
typedef struct {
    float accel[3];   // X/Y/Z 加速度 (g)
    float gyro[3];    // X/Y/Z 角速度 (°/s)
    float angle[3];   // Roll/Pitch/Yaw 角度 (°)
} IMU_Data;

bool Gyroscpe_Data_Ready();
void Gyroscpe_Stop();
void Gyroscpe_Start();
void Gyroscpe_Init();
bool Gyroscpe_Data_Ready();
void parse_wt61_data(void);
void send_cmd(const uint8_t *cmd, uint8_t len);

// 全局变量定义

extern volatile uint8_t wt61_rx_buffer[WT61_FRAME_LENGTH];
extern volatile uint8_t wt61_rx_index;
extern volatile uint8_t wt61_data_ready ;

extern volatile IMU_Data imu_data;


#endif  