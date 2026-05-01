#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Move.h"
#include "OLED.h"

uint8_t Serial_RxFlag;
int8_t Serial_RxPacket[3]={0, 0, 0};
//	x	y	check bit

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag==1)
	{
		Serial_RxFlag=0;
		return 1;
	}
	return 0;
}

void BT_control(void)
{
	switch(Serial_RxPacket[0])
	{
		case -10 ... 10:		//x=0
			switch(Serial_RxPacket[1])
			{
				case -10 ... 10:
					Stop();
				break;
				case 11 ... 50:
					Run_forward();
				break;
				case -50 ... -11:
					Run_backward();
				break;
			};
		break;
		
		case -50 ... -11:		//x>0
			switch(Serial_RxPacket[1])
			{
				case -50 ... 50:
					Other_direction();
				break;
			};
		break;
		
		case 11 ... 50:	//x<0
			switch(Serial_RxPacket[1])
			{
				case -50 ... 50:
					Other_direction();
				break;
			};
		break;
	}
}

void USART1_IRQHandler(void)
{
	static uint8_t RxState=0;
	static uint8_t pRxPacket=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		uint8_t RxData=USART_ReceiveData(USART1);
		if(RxState==0)
		{
			if(RxData==0xA5)
			{
				RxState=1;
				pRxPacket=0;
			}
		}
		else if(RxState==1)
		{
			Serial_RxPacket[pRxPacket]=RxData;
			pRxPacket++;
			if(pRxPacket>=3)
			{
				RxState=2;
			}
		}
		else if(RxState==2)
		{
			if(RxData==0x5A)
			{
				RxState=0;
				Serial_RxFlag=1;
			}
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
