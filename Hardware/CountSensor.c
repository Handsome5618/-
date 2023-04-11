#include "stm32f10x.h"                  // Device header
#include "Buzzer.h"
#include "Delay.h"

void CountSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//∏°ø’ ‰»Î
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
{
	static u8 BuzzerFlag = 0;
	if (EXTI_GetITStatus(EXTI_Line14) == SET && BuzzerFlag == 0)
	{
		delay_ms(200);
		if(EXTI_GetITStatus(EXTI_Line14) == SET && BuzzerFlag == 0)
		{
		Buzzer_ON();
		EXTI_ClearITPendingBit(EXTI_Line14);
		BuzzerFlag = 1;
		}
	}
	else if(EXTI_GetITStatus(EXTI_Line14) == SET && BuzzerFlag == 1)
	{
		delay_ms(200);
		if(EXTI_GetITStatus(EXTI_Line14) == SET && BuzzerFlag == 1)
		{
		Buzzer_OFF();
		EXTI_ClearITPendingBit(EXTI_Line14);
		BuzzerFlag = 0;
		}
	}
}
