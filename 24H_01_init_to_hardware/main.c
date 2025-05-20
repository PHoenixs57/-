#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include "motor.h"
#include "bluetooth.h"
#include "delay.h"
#include "pid.h"

int main(void)
{
    SYSCFG_DL_init();
    // 电机初始化
    motor_init();
    motor_duty(PWM0_TIM, PWM0_CHANNEL, 100); // 设置电机速度
    motor_duty(PWM1_TIM, PWM1_CHANNEL, 100);
    // 增加外部中断代码
    NVIC_EnableIRQ(ENCODER_INT_IRQN);
    // 清除定时器中断标志 Clear the timer interrupt flag
    NVIC_ClearPendingIRQ(ENCODER_TIM0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(ENCODER_TIM1_INST_INT_IRQN);
    // 使能定时器中断 Enable timer interrupt
    NVIC_EnableIRQ(ENCODER_TIM0_INST_INT_IRQN);
    NVIC_EnableIRQ(ENCODER_TIM1_INST_INT_IRQN);
    // 清除串口中断标志 Clear the serial port interrupt flag
    NVIC_ClearPendingIRQ(Bluetooth_INST_INT_IRQN);
    // 使能串口中断 Enable serial port interrupt
    NVIC_EnableIRQ(Bluetooth_INST_INT_IRQN);

    // 开始运行主程序
    uart0_send_string("hello\r\n");
    // pid_init(&motorA, LEFT_MOTOR, DELTA_PID, )
    motorA.target = 0;

    while (1) {
        motor_duty(PWM0_TIM, PWM0_CHANNEL, motorA.target);
        // pid_cal(motorA);
    }
}
