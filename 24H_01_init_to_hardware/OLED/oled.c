#include "oled.h"
#include <stdint.h>
#include "codetab.h"
#include "delay.h"
#include "ti_msp_dl_config.h"

//向OLED寄存器地址写一个byte的数据	Write a byte of data to the OLED register address
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
    uint8_t temp[2];
    temp[0] = addr;
    temp[1] = data;
 
    DL_I2C_fillControllerTXFIFO(I2C_0_INST, temp, 2);
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
 
    DL_I2C_startControllerTransfer(I2C_0_INST, OLED_ADDRESS, DL_I2C_CONTROLLER_DIRECTION_TX, 2);
    while (DL_I2C_getControllerStatus(I2C_0_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
    while (!(DL_I2C_getControllerStatus(I2C_0_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));
    DL_I2C_flushControllerTXFIFO(I2C_0_INST);
}

//写指令		write command
void WriteCmd(unsigned char I2C_Command)
{
		I2C_WriteByte(0x00,I2C_Command);
}

//写数据		Write Data
void WriteData(unsigned char I2C_Data)
{
		I2C_WriteByte(0x40,I2C_Data);
}

//厂家初始化代码		Factory initialization code
void OLED_Init(void)
{
		delay_ms(100);
		WriteCmd(0xAE); //display off
		WriteCmd(0x20);	//Set Memory Addressing Mode	
		WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
		WriteCmd(0xc8);	//Set COM Output Scan Direction
		WriteCmd(0x00); //---set low column address
		WriteCmd(0x10); //---set high column address
		WriteCmd(0x40); //--set start line address
		WriteCmd(0x81); //--set contrast control register
		WriteCmd(0xff); //áá?èμ÷?ú 0x00~0xff
		WriteCmd(0xa1); //--set segment re-map 0 to 127
		WriteCmd(0xa6); //--set normal display
		WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
		WriteCmd(0x3F); //
		WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
		WriteCmd(0xd3); //-set display offset
		WriteCmd(0x00); //-not offset
		WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
		WriteCmd(0xf0); //--set divide ratio
		WriteCmd(0xd9); //--set pre-charge period
		WriteCmd(0x22); //
		WriteCmd(0xda); //--set com pins hardware configuration
		WriteCmd(0x12);
		WriteCmd(0xdb); //--set vcomh
		WriteCmd(0x20); //0x20,0.77xVcc
		WriteCmd(0x8d); //--set DC-DC enable
		WriteCmd(0x14); //
		WriteCmd(0xaf); //--turn on oled panel

}

//设置光标起始坐标（x,y）		Set the cursor starting coordinates (x, y)
void OLED_SetPos(unsigned char x,unsigned char y)
{
		WriteCmd(0xb0+y);
		WriteCmd( (x & 0xf0) >> 4 | 0x10 );
		WriteCmd( (x & 0x0f) | 0x01 );
}

//填充整个屏幕	Fill the entire screen
void OLED_Fill(unsigned char Fill_Data)
{
		unsigned char m,n;
		
		for(m=0;m<8;m++)
		{
				WriteCmd(0xb0+m);
				WriteCmd(0x00);
				WriteCmd(0x10);
				
				for(n=0;n<128;n++)
				{
						WriteData(Fill_Data);
				}
		}
}

//清屏	clear screen
void OLED_CLS(void)
{
		OLED_Fill(0x00);
}

//将OLED从休眠中唤醒		Wake up the OLED from sleep
void OLED_ON(void)
{
		WriteCmd(0xAF);
		WriteCmd(0x8D);
		WriteCmd(0x14);
}

//让OLED休眠 -- 休眠模式下,OLED功耗不到10uA	Put OLED to sleep -- In sleep mode, OLED consumes less than 10uA
void OLED_OFF(void)
{
		WriteCmd(0xAE);
		WriteCmd(0x8D);
		WriteCmd(0x10);
}

void OLED_ShowStr(unsigned char x,unsigned char y,unsigned char ch[],unsigned char TextSize)
{
		unsigned char c = 0,i = 0,j = 0;
		
		switch(TextSize)
		{
			case 1:
			{
					while(ch[j] != '\0')
					{
							c = ch[j] - 32;
							if(x>126)
							{
									x = 0;
									y++;
							}
							
							OLED_SetPos(x,y);
							
							for(i=0;i<6;i++)
							{
									WriteData(F6x8[c][i]);
							}
							x+=6;
							j++;
					}
			}
			break;
			
			case 2:
			{
					while(ch[j] != '\0')
					{
							c = ch[j] - 32;
							
							if(x>120)
							{
									x = 0;
									y++;
							}
							
							OLED_SetPos(x,y);
							
							for(i=0;i<8;i++)
							{
									WriteData(F8X16[c*16+i]);	
							}
							
							OLED_SetPos(x,y+1);
							
							for(i=0;i<8;i++)
							{
									WriteData(F8X16[c*16+i+8]);
							}
							x+=8;
							j++;		
					}
			}
			break;
		}
}

