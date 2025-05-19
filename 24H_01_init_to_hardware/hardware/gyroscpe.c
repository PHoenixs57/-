#include "gyroscpe.h"




// 全局变量定义

volatile uint8_t wt61_rx_buffer[WT61_FRAME_LENGTH];
volatile uint8_t wt61_rx_index = 0;
volatile uint8_t wt61_data_ready = 0x00;


volatile IMU_Data imu_data;


//11byte -- 1帧 


const uint8_t Z_angle_reset                [3] = {0xFF,0xAA,0x52};
const uint8_t Toggle_sleep_and_active_mode [3] = {0xFF,0xAA,0x60};
const uint8_t Serial_mode                  [3] = {0xFF,0xAA,0x61};
const uint8_t Baud_115200                  [3] = {0xFF,0xAA,0x63};
const uint8_t Horizontal_installation      [3] = {0xFF,0xAA,0x65};

void Gyroscpe_Init()
{
    /* Configure DMA source, destination and size */
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(&Gyroscpe_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &wt61_rx_buffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, UART_Gyroscpe_PACKET_SIZE);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);
     while (false == DL_DMA_isChannelEnabled(DMA, DMA_CH1_CHAN_ID)) {
        __BKPT(0);
    }
}

void Gyroscpe_Start()
{
    //NVIC_ClearPendingIRQ(UART1_INT_IRQn);
    NVIC_EnableIRQ(UART1_INT_IRQn);
    wt61_data_ready = 0x00;
}

void Gyroscpe_Stop()
{
    NVIC_DisableIRQ(UART1_INT_IRQn);
}

bool Gyroscpe_Data_Ready()
{
    return (wt61_data_ready == 0x07);
}
    

   /* send_`cmd(Horizontal_installation,3);
    delay_ms(100); // 增加延时
    send_cmd(Z_angle_reset,3);
    delay_ms(100);
    send_cmd(Serial_mode,3);
    delay_ms(100);
    send_cmd(Baud_115200,3);
    delay_ms(100);*/

    // while (1)
    // {
    //     if (wt61_data_ready) {
    //         wt61_data_ready = false;
    //         char tx_buffer[128];
    //         int len = snprintf(tx_buffer, sizeof(tx_buffer),
    //             "Accel: X=%.2f Y=%.2f Z=%.2f\r\n"
    //             "Gyro: X=%.2f Y=%.2f Z=%.2f\r\n",
    //             imu_data.accel[0], imu_data.accel[1], imu_data.accel[2],
    //             imu_data.gyro[0], imu_data.gyro[1], imu_data.gyro[2]);
            
    //         // 分段发送防止阻塞
    //         for (int i = 0; i < len; i++) {
    //             uart0_send_char(tx_buffer[i]);
    //         }
    //     }
    // }









void send_cmd(const uint8_t *cmd, uint8_t len)
{
     for(uint8_t i=0; i<len; i++) {
        DL_UART_Main_transmitData(UART1,cmd[i]); 
    }
}

/* 数据解析函数 */
void parse_wt61_data(void)
{
    /* 数据解析 */
    switch(wt61_rx_buffer[1])  // TYPE字段
    {
        case WT61_TYPE_ACCEL:  // 加速度
            imu_data.accel[0] = (int16_t)((wt61_rx_buffer[3]<<8)|wt61_rx_buffer[2]) / 32768.0f * 16.0f;
            imu_data.accel[1] = (int16_t)((wt61_rx_buffer[5]<<8)|wt61_rx_buffer[4]) / 32768.0f * 16.0f;
            imu_data.accel[2] = (int16_t)((wt61_rx_buffer[7]<<8)|wt61_rx_buffer[6]) / 32768.0f * 16.0f;
            wt61_data_ready |= 0x04;
            break;
            
        case WT61_TYPE_GYRO:   // 角速度
            imu_data.gyro[0] = (int16_t)((wt61_rx_buffer[3]<<8)|wt61_rx_buffer[2]) / 32768.0f * 2000.0f;
            imu_data.gyro[1] = (int16_t)((wt61_rx_buffer[5]<<8)|wt61_rx_buffer[4]) / 32768.0f * 2000.0f;
            imu_data.gyro[2] = (int16_t)((wt61_rx_buffer[7]<<8)|wt61_rx_buffer[6]) / 32768.0f * 2000.0f;
            wt61_data_ready |= 0x02;
            break;
            
        case WT61_TYPE_ANGLE:  // 角度
            imu_data.angle[0] = (int16_t)((wt61_rx_buffer[3]<<8)|wt61_rx_buffer[2]) / 32768.0f * 180.0f;
            imu_data.angle[1] = (int16_t)((wt61_rx_buffer[5]<<8)|wt61_rx_buffer[4]) / 32768.0f * 180.0f;
            imu_data.angle[2] = (int16_t)((wt61_rx_buffer[7]<<8)|wt61_rx_buffer[6]) / 32768.0f * 180.0f;
            wt61_data_ready |= 0x01;
            break;
    }
}