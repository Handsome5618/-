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
    u8 BoxId;			//锁ID
    u8 BoxIdFlat;		//是否被选上标志位
    u8 PassFlag;		//是否已经设置密码标志位
    u8 PassWord[6];		//密码
    u8 BoxFunction;		//功能标志位（存或取）
	u8 PassJudgeFlat;	//判断密码输入是否正确标志位
	u8 OpenFlag;		//开箱方式
	u8 FaceFlag;		//判断人脸识别
	u8 FaceWord;		//人脸密码
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
        PC_SendString((u8*)"\n\n连接成功\n");
        PC_SendString((u8*)"欢迎使用“临存”智能寄存终端\n");
        PC_SendString((u8*)"请选择锁\n\n");
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
            PC_SendString((u8*)"\n\n请选择功能：\n存/取\n\n");
            while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			Box[1].BoxFunction = num;
            delay_ms(10);
			PC_SendString((u8*)"\n\n请选择解锁方式：\n1.密码\n2.人脸\n\n");
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
			PC_SendString((u8*)"\n\n正在录入人脸数据\n\n");
			while(RxFlag1 == 0)
            {
                continue;
            }
			RxFlag1 = 0;
			delay_ms(10);
			PC_SendString((u8*)"\n\n人脸录入成功\n");
			delay_ms(10);
			PC_SendString((u8*)"锁已打开");
			delay_ms(10);
			PC_SendString((u8*)"请存放您的物品\n");
			delay_ms(10);
			PC_SendString((u8*)"按确认键结束\n\n");          
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
            PC_SendString((u8*)"\n\n您的物品已储存妥当\n");
            PC_SendString((u8*)"锁已关闭\n\n");
			Box[1].BoxIdFlat = 0;
		}
		
		else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].FaceFlag == 1&& Box[1].BoxFunction == 2)
		{
			PC_SendString((u8*)"\n\n    锁一    \n");
            PC_SendString((u8*)"正在识别你\n请将您的脸正能对摄像头\n\n");
			while(RxFlag1 == 0)
            {
                continue;
            }
			RxFlag1 = 0;
			delay_ms(10);
			Box1_Open();
			PC_SendString((u8*)"\n\n人脸识别成功\n");
			PC_SendString((u8*)"锁已打开,请取走您的物品\n");
			PC_SendString((u8*)"按确认键结束\n\n");
			 while(flat == 0)
            {
                continue;
            }
            flat = 0;
            delay_ms(10);
			Box1_Close();
			PC_SendString((u8*)"\n\n锁已关闭\n\n");            
		}
		
        else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].PassFlag == 0 && Box[1].BoxFunction == 1 && Box[1].OpenFlag == 1)
        {
            PC_SendString((u8*)"\n\n    锁一    \n");
            PC_SendString((u8*)"请设置您的密码\n\n");
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
            PC_SendString((u8*)"\n\n密码设置成功\n\n");
            Box[1].PassFlag = 1;
			PassFlag = 0;
            Box1_Open();
            PC_SendString((u8*)"\n\n锁已打开，请存放您的物品\n");
            PC_SendString((u8*)"按确认键结束\n\n");
            while(flat == 0)
            {
                continue;
            }
            flat = 0;
            delay_ms(10);
            Box1_Close();
            PC_SendString((u8*)"\n\n您的物品已储存妥当\n");
            PC_SendString((u8*)"锁已关闭\n\n");
			Box[1].BoxIdFlat = 0;
        }
		else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].PassFlag == 1&& Box[1].BoxFunction == 2)
		{
			PC_SendString((u8*)"\n\n请输入您设置的密码\n\n");
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
				PC_SendString((u8*)"\n\n密码有误\n请重新输入\n\n");
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
			PC_SendString((u8*)"\n\n密码正确  锁已打开\n请取走您的物品");
			PC_SendString((u8*)"\n按确认键结束\n\n");
			flat = 0;
			while(flat == 0)
            {
                continue;
            }
            flat = 0;
			delay_ms(10);
			Box1_Close();
			PC_SendString((u8*)"\n\n锁已关闭\n\n");
			Box[1].PassFlag = 0;
		}
		else if(Box[1].BoxId == 1 && Box[1].BoxIdFlat == 1 && Box[1].PassFlag == 1 && Box[1].BoxFunction == 1)
		{
			PC_SendString((u8*)"\n\n该锁已被使用\n");
			PC_SendString((u8*)"请选择别的锁\n\n");
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
			PC_SendString((u8*)"\n\n该锁未存放东西\n");
			PC_SendString((u8*)"请选择功能：\n存/取\n\n");
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



void Set_Box1_PassWord(u8 i)		//设置密码
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
