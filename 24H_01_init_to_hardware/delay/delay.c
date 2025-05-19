#include "delay.h"

volatile unsigned int delay_times = 0;

void delay_us(unsigned long __us) 
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 38;

    // ������Ҫ��ʱ���� = �ӳ�΢���� * ÿ΢���ʱ����
	//The number of clocks required for calculation = the number of microseconds of delay * the number of clocks per microsecond
    ticks = __us * (32000000 / 1000000);

    // ��ȡ��ǰ��SysTickֵ
	//Get the current SysTick value
    told = SysTick->VAL;

    while (1)
    {
        // �ظ�ˢ�»�ȡ��ǰ��SysTickֵ
		//Repeatedly refresh to get the current SysTick value
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;

            // ����ﵽ����Ҫ��ʱ���������˳�ѭ��
			//If the required number of clocks is reached, exit the loop
            if (tcnt >= ticks)
                break;
        }
    }
}
//����δ�ʱ��ʵ�ֵľ�ȷms��ʱ
//Accurate ms delay with tick timer
void delay_ms(unsigned long ms) 
{
	delay_us( ms * 1000 );
}

//�δ�ʱ���жϴ���	Tick timer interrupt handling
void SysTick_Handler(void)
{
	if(delay_times != 0)
	{
		delay_times--;
	}
}
