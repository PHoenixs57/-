#ifndef __PID_H
#define __PID_H
#include "motor.h"
#include "ti_msp_dl_config.h"
#include "ti/driverlib/m0p/dl_core.h"

enum
{
    POSITION_PID = 0, // 位置式
    DELTA_PID    // 增量式
};


typedef struct
{
    Motor_ID ID;
	float target;	
	float now;
	float error[3];		
	float p,i,d;
	float pout, dout, iout;
	float out;
	
	int pid_mode;

}pid_t;

void pid_init(pid_t *pid, Motor_ID id, uint32_t mode, float p, float i, float d);
void motor_target_set(pid_t *pid, int speed);
void pid_control();
void pid_cal(pid_t *pid);

extern pid_t motorA, motorB; // A为左，B为右

#endif
