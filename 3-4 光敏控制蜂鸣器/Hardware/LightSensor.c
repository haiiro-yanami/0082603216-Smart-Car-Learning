#include "stm32f10x.h"                  // Device header
void LS_Init(void)
{
	//开启时钟，不然一切免谈
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//配置Pin口，输入输出模式，速度
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//配置好了你得告诉他配置给哪个GPIO
	GPIO_Init(GPIOB , &GPIO_InitStructure);
	
}


uint8_t LS_GetNum(void)
{
	return GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_13);
}
