#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdarg.h>
#include "pwm.h"
#include "MG996r.h"
#include "usart3.h"
#include "each.h"
#include "CountSensor.h"
#include "Buzzer.h"
#include "usart1.h"

#define PassLen 4

extern u8 RxFlag1;
extern u8 RxBuffer1[10];	

u8 BoxId = 0;
u8 flat = 0;
u8 num;
u8 PassFlag = 0;
u8 Date[4] = {0};
u8 Box1_PassWord[PassLen] = {0};
u8 Box2_PassWord[PassLen] = {0};


void Box_Init(void);
void Set_Box1_PassWord(u8 i);
u8 PsaaWord_Judge(void);
void Get_Pass(void);

struct BOX
{
    u8 BoxId;			//��ID
    u8 BoxIdFlat;		//�Ƿ�ѡ�ϱ�־λ
    u8 PassFlag;		//�Ƿ��Ѿ����������־λ
    u8 PassWord[6];		//����
    u8 BoxFunction;		//���ܱ�־λ�����ȡ��
	u8 PassJudgeFlat;	//�ж����������Ƿ���ȷ��־λ
	u8 OpenFlag;		//���䷽ʽ
	u8 FaceFlag;		//�ж�����ʶ��
	u8 FaceWord;		//��������
} Box[5];

int main(void)
{
	CountSensor_Init();
    delay_init();
    USART3_Config();
    TIM4_PWM_Init(59999,24);
    Box_Init();
	Buzzer_Init();
	USART1_Init();
    while(1)
    {
        while(flat == 0)
        {
            continue;
        }
        flat = 0;
        delay_ms(10);
        PC_SendString((u8*)"\n\n���ӳɹ�\n");
        PC_SendString((u8*)"��ӭʹ�á��ٴ桱���ܼĴ��ն�\n");
        PC_SendString((u8*)"��ѡ����\n\n");
        while(flat == 0)
        {
            continue;
        }
        flat = 0;
        delay_ms(10);
        BoxId = num;
		Box[BoxId].BoxIdFlat = 1;
        if(Box[1].BoxId == BoxId)
        {
            PC_SendString((u8*)"\n\n��ѡ���ܣ�\n��/ȡ\n\n");
            while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			Box[1].BoxFunction = num;
            delay_ms(10);
			PC_SendString((u8*)"\n\n��ѡ�������ʽ��\n1.����\n2.����\n\n");
			while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			Box[1].OpenFlag = num;
        }	
		if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].FaceFlag == 0 && Box[1].BoxFunction == 1 && Box[1].OpenFlag == 2)
		{
			PC_SendString((u8*)"\n\n����¼����������\n\n");
			while(RxFlag1 == 0)
            {
                continue;
            }
			RxFlag1 = 0;
			delay_ms(10);
			PC_SendString((u8*)"\n\n����¼��ɹ�\n");
			delay_ms(10);
			PC_SendString((u8*)"���Ѵ�");
			delay_ms(10);
			PC_SendString((u8*)"����������Ʒ\n");
			delay_ms(10);
			PC_SendString((u8*)"��ȷ�ϼ�����\n\n");          
			Box[1].FaceFlag = 1;
			Box[1].FaceWord = RxBuffer1[2];
			Box1_Open();
			 while(flat == 0)
            {
                continue;
            }
            flat = 0;
            delay_ms(10);
            Box1_Close();
            PC_SendString((u8*)"\n\n������Ʒ�Ѵ����׵�\n");
            PC_SendString((u8*)"���ѹر�\n\n");
			Box[1].BoxIdFlat = 0;
		}
		
		else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].FaceFlag == 1&& Box[1].BoxFunction == 2)
		{
			PC_SendString((u8*)"\n\n    ��һ    \n");
            PC_SendString((u8*)"����ʶ����\n�뽫���������ܶ�����ͷ\n\n");
			while(RxFlag1 == 0)
            {
                continue;
            }
			RxFlag1 = 0;
			delay_ms(10);
			Box1_Open();
			PC_SendString((u8*)"\n\n����ʶ��ɹ�\n");
			PC_SendString((u8*)"���Ѵ�,��ȡ��������Ʒ\n");
			PC_SendString((u8*)"��ȷ�ϼ�����\n\n");
			 while(flat == 0)
            {
                continue;
            }
            flat = 0;
            delay_ms(10);
			Box1_Close();
			PC_SendString((u8*)"\n\n���ѹر�\n\n");            
		}
		
        else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].PassFlag == 0 && Box[1].BoxFunction == 1 && Box[1].OpenFlag == 1)
        {
            PC_SendString((u8*)"\n\n    ��һ    \n");
            PC_SendString((u8*)"��������������\n\n");
			PassFlag = 1;
            while(flat == 0)
            {
                continue;
            }
            flat = 0;
            delay_ms(10);
            while(flat == 0)
            {
                continue;
            }
            flat = 0;
            delay_ms(10);
            Set_Box1_PassWord(Box[1].BoxId);
            PC_SendString((u8*)"\n\n�������óɹ�\n\n");
            Box[1].PassFlag = 1;
			PassFlag = 0;
            Box1_Open();
            PC_SendString((u8*)"\n\n���Ѵ򿪣�����������Ʒ\n");
            PC_SendString((u8*)"��ȷ�ϼ�����\n\n");
            while(flat == 0)
            {
                continue;
            }
            flat = 0;
            delay_ms(10);
            Box1_Close();
            PC_SendString((u8*)"\n\n������Ʒ�Ѵ����׵�\n");
            PC_SendString((u8*)"���ѹر�\n\n");
			Box[1].BoxIdFlat = 0;
        }
		else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].PassFlag == 1&& Box[1].BoxFunction == 2)
		{
			PC_SendString((u8*)"\n\n�����������õ�����\n\n");
			PassFlag = 1;
			while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			PassFlag = 0;
			Get_Pass();
			while((PsaaWord_Judge()) == 0)
			{
				PC_SendString((u8*)"\n\n��������\n����������\n\n");
				PassFlag = 1;
				while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			PassFlag = 0;
			Get_Pass();			
			}
			Box1_Open();
			PC_SendString((u8*)"\n\n������ȷ  ���Ѵ�\n��ȡ��������Ʒ");
			PC_SendString((u8*)"\n��ȷ�ϼ�����\n\n");
			flat = 0;
			while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			Box1_Close();
			PC_SendString((u8*)"\n\n���ѹر�\n\n");
			Box[1].PassFlag = 0;
		}
		else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].PassFlag == 1 && Box[1].BoxFunction == 1)
		{
			PC_SendString((u8*)"\n\n�����ѱ�ʹ��\n");
			PC_SendString((u8*)"��ѡ������\n\n");
			while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			Box[1].BoxFunction = num;
            delay_ms(10);
		}
		else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].PassFlag == 0 && Box[1].BoxFunction == 2)
		{
			PC_SendString((u8*)"\n\n����δ��Ŷ���\n");
			PC_SendString((u8*)"��ѡ���ܣ�\n��/ȡ\n\n");
			while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			Box[1].BoxFunction = num;
            delay_ms(10);
		}
	}
}



void Set_Box1_PassWord(u8 i)		//��������
{
	Box1_PassWord[0] = Date[0];
	Box1_PassWord[1] = Date[1];
	Box1_PassWord[2] = Date[2];
    Box[i].PassWord[0] = Box1_PassWord[0];
    Box[i].PassWord[1] = Box1_PassWord[1];
    Box[i].PassWord[2] = Box1_PassWord[2];
}

void Box_Init()
{
    Box[1].BoxId = 1;
    Box[1].BoxIdFlat = 0;
    Box[1].PassFlag = 0;
    Box[1].PassWord[0] = 0;
    Box[1].PassWord[1] = 0;
    Box[1].PassWord[2] = 0;
    Box[1].PassWord[3] = 0;
    Box[1].PassWord[4] = 0;
    Box[1].PassWord[5] = 0;
    Box[1].BoxFunction = 0;
	Box[1].PassJudgeFlat = 0;
	Box[1].FaceFlag = 0;

    Box[2].BoxId = 2;
    Box[1].BoxIdFlat = 0;
    Box[2].PassFlag = 0;
    Box[2].PassWord[0] = 0;
    Box[2].PassWord[1] = 0;
    Box[2].PassWord[2] = 0;
    Box[2].PassWord[3] = 0;
    Box[2].PassWord[4] = 0;
    Box[2].PassWord[5] = 0;
    Box[2].BoxFunction = 0;
	Box[2].PassJudgeFlat = 0;
}

void Get_Pass()
{
	Box1_PassWord[0] = Date[0];
	Box1_PassWord[1] = Date[1];
	Box1_PassWord[2] = Date[2];
}

u8 PsaaWord_Judge(void)
{
	if(Box1_PassWord[0] == Box[1].PassWord[0] && Box1_PassWord[1] == Box[1].PassWord[1] && Box1_PassWord[2] == Box[1].PassWord[2])
	{
		Box[1].PassJudgeFlat = 1;
	}
	else
	{
		Box[1].PassJudgeFlat = 0;
	}
	return Box[1].PassJudgeFlat;
}
