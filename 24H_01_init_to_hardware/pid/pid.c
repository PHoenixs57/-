#include "pid.h"
#include "motor.h"


pid_t motorA, motorB;

/**
 * @brief 初始化 PID 控制器参数
 * 
 * @param pid     PID 结构体指针 (motorA / motorB)
 * @param id      电机 ID（LEFT_MOTOR / RIGHT_MOTOR）
 * @param mode    PID 模式（DELTA_PID / POSITION_PID）
 * @param p       比例增益
 * @param i       积分增益
 * @param d       微分增益
 * 
 * @note 
 * - 示例: pid_init(&motorA, LEFT_MOTOR, DELTA_PID, 0.9, 0.4, 0.1);
 */
void pid_init(pid_t *pid, Motor_ID id, uint32_t mode, float p, float i, float d)
{
    pid->ID       = id;
    pid->pid_mode = mode;
    pid->p        = p;
    pid->i        = i;
    pid->d        = d;
}

/**
 * @brief 设置目标速度并更新电机方向
 * 
 * @param pid   PID 结构体指针
 * @param speed 目标速度（正/负表示方向）
 * 
 * @note 
 * - 速度值会被截断为整数（`int` 类型）。
 * - `motor[pid->ID].motor_dir` 用于记录方向（1: 正向，0: 反向）。
 * - 若需支持小数速度，建议修改参数类型为 `float` 并调整内部处理逻辑。
 */
void motor_target_set(pid_t *pid, int speed)
{
    if (speed >= 0) // 速度为正向
    {
        pid->target = (int)speed;
        motor[pid->ID].motor_dir = 1;
    }
    else {
        pid->target = -(int)speed;
        motor[pid->ID].motor_dir = 0;
    }
}

/**
 * @brief 执行 PID 控制流程
 * 
 * @details 
 * 1. 根据电机方向获取当前速度（编码器计数值）。
 * 2. 调用 `pid_cal()` 计算 PID 输出。
 * 3. 将输出值写入 PWM 通道控制电机。
 * 
 * @note 
 * - 该函数需在主循环中定期调用（如定时器中断）。
 * - `MAX_DUTY` 需在代码中定义，表示 PWM 占空比上限。
 */
void pid_control()
{
    // 1.获取当前速度
    if (motor[LEFT_MOTOR].motor_dir) { motorA.now = motor[LEFT_MOTOR].encoder_count; }
    else  { motorA.now = -motor[LEFT_MOTOR].encoder_count; }
    if (motor[RIGHT_MOTOR].motor_dir) { motorB.now = motor[RIGHT_MOTOR].encoder_count; }
    else  { motorB.now = -motor[RIGHT_MOTOR].encoder_count; }
    // 2.输入PID控制器进行计算
    pid_cal(&motorA);
    pid_cal(&motorB);
    // 3.将计算结果输出进电机
    motor_duty(PWM0_TIM, PWM0_CHANNEL, motorA.out);
    motor_duty(PWM1_TIM, PWM1_CHANNEL, motorB.out);
}

/**
 * @brief PID 控制核心计算逻辑
 * 
 * @param pid PID 结构体指针
 * 
 * @details 
 * - 支持增量式（DELTA_PID）和位置式（POSITION_PID）两种模式。
 * - 误差历史存储在 `pid->error[2]` 中。
 * - 输出值 `pid->out` 会进行限幅处理（0 ~ MAX_DUTY）。
 * 
 * @note 
 * - 积分项需注意饱和问题，建议增加积分限幅逻辑。
 * - 微分项对噪声敏感，可考虑加入滤波处理。
 */
void pid_cal(pid_t *pid)
{
	// 计算当前偏差
	pid->error[0] = pid->target - pid->now;

	// 计算输出
	if(pid->pid_mode == DELTA_PID)  // 增量式
	{
		pid->pout = pid->p * (pid->error[0] - pid->error[1]);
		pid->iout = pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - 2 * pid->error[1] + pid->error[2]);
		pid->out += pid->pout + pid->iout + pid->dout;
	}
	else if(pid->pid_mode == POSITION_PID)  // 位置式
	{
		pid->pout = pid->p * pid->error[0];
		pid->iout += pid->i * pid->error[0];
		pid->dout = pid->d * (pid->error[0] - pid->error[1]);
		pid->out = pid->pout + pid->iout + pid->dout;
	}

	// 记录前两次偏差
	pid->error[2] = pid->error[1];
	pid->error[1] = pid->error[0];

	// 输出限幅
	if(pid->out>=MAX_DUTY)	
		pid->out=MAX_DUTY;
	if(pid->out<=0)	
		pid->out=0;
}
