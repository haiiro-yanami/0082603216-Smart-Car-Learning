#include "stm32f10x.h"                  // Device header
#include "Delay.h"
const int shanshuo = 80;
void liang(void);
void an(void);
void qianbanliang(void);
void houbanliang(void);

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ; 

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	while (1)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)0);
		qianbanliang();
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)0);
		houbanliang();
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)1);
		an();
	}
}

void qianbanliang()
{
	GPIO_Write(GPIOA, 0x00FF);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x007F);
	Delay_ms(shanshuo);
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)1);
	GPIO_Write(GPIOA, 0x003F);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x001F);
	Delay_ms(shanshuo);
}
void houbanliang()
{
	GPIO_Write(GPIOA, 0x000F);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x0007);
	Delay_ms(shanshuo);
	GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)0);
	GPIO_Write(GPIOA, 0x0003);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x0001);
	Delay_ms(shanshuo);
}
void liang()
{
	GPIO_Write(GPIOA, 0x00FF);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x007F);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x003F);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x001F);
	Delay_ms(shanshuo);
	
	GPIO_Write(GPIOA, 0x000F);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x0007);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x0003);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x0001);
	Delay_ms(shanshuo);
}

void an()
{
	GPIO_Write(GPIOA, 0x0000);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x0080);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x00C0);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x00E0);
	Delay_ms(shanshuo);
	
	GPIO_Write(GPIOA, 0x00F0);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x00F8);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x00FC);
	Delay_ms(shanshuo);
	GPIO_Write(GPIOA, 0x00FE);
	Delay_ms(shanshuo);
}
