#include "stm32f10x.h"			// Device header	
#include "mg996r.h"

void MG996r_Angle(u16 angle)
{
    TIM_SetCompare3(TIM4,angle);
}

void Box1_Open(void)
{
	 TIM_SetCompare3(TIM4,1500);
}

void Box1_Close(void)
{
	 TIM_SetCompare3(TIM4,7500);
}

void Box2_Open(void)
{
	 TIM_SetCompare4(TIM4,2000);
}

void Box2_Close(void)
{
	TIM_SetCompare4(TIM4,6700);
}
	
