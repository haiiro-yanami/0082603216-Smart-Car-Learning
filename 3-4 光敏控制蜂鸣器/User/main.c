#include "stm32f10x.h"                  // Device header

#include "Buzzer.h"
#include "LightSensor.h"


int main(void)
{
	BUZZER_Init();
	LS_Init();
		
	while (1)
	{
		if(LS_GetNum() == 1)
		{
			BUZZER_ON();
		}
		else
		{
			BUZZER_OFF();
		}
	}
}
