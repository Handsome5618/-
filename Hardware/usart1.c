#include "stm32f10x.h"                  // Device header

u8 com_data; 
u8 i;
u8 RxBuffer1[10]={0};	
u8 RxFlag1 = 0;

void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;//���ڲ������ýṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;//�����ж����ýṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//�򿪴��ڸ���ʱ��
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //��PC�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�趨IO�ڵ�����ٶ�Ϊ50MHz
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOC.10

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA.10
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOC.10  

  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
  	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����Ϊ115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽʹ��
	USART_Init(USART1, &USART_InitStructure); //��ʼ������4
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1
   	USART_ClearFlag(USART1, USART_FLAG_TC);       //�崮��1���ͱ�־
}

void Openmv_Receive_Data(int16_t data)
{
	static u8 state = 0;	
	if(state==0&&data==0x2C)
	{
		state=1;
		RxBuffer1[0]=data;
	}
	else if(state==1&&data==0x12)
	{
		state=2;
		RxBuffer1[1]=data;
	}
	else if(state==2)
	{
		RxBuffer1[2]=data;
		state=3;
	}
	else if(state==3)
	{
		RxBuffer1[3]=data;
		state=4;
	}
	else if(state==4)
	{
		RxBuffer1[4]=data;
		state=5;
	}
	else if(state==5)
	{
		RxBuffer1[5]=data;
		state=6;
	}
	else if(state==6)		//state == 3  ����Ƿ���ܵ�������־
	{
			RxBuffer1[6]=data;
        	if(RxBuffer1[6] == 0x5B)
        	{
            	state = 0;
				RxFlag1 = 1;
        	}
        	else   //wrong thing
        	{
            	state = 0;
        	}
	}    
	else	//wrong thing
	{
		state = 0;
	}
}

void USART1_IRQHandler(void)			   //�����ж�
{
	u8 temp;
	if( USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET )
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//����жϱ�־
		temp = USART_ReceiveData(USART1);
		Openmv_Receive_Data(temp);//openmv���ݴ�����
	}
}

