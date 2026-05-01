#include "stm32f10x.h"                  // Device header

int16_t Speed_Right=0;
int16_t Speed_Left=0;

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_IC_InitStructure;
	TIM_ICStructInit(&TIM_IC_InitStructure);
	TIM_IC_InitStructure.TIM_Channel = TIM_Channel_1;
	TIM_IC_InitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3,&TIM_IC_InitStructure);
	TIM_ICInit(TIM4,&TIM_IC_InitStructure);
	
	TIM_IC_InitStructure.TIM_Channel = TIM_Channel_2;
	TIM_IC_InitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3,&TIM_IC_InitStructure);
	TIM_ICInit(TIM4,&TIM_IC_InitStructure);
	
	//上升沿代表不反相
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}

//TIM3	J1	Right
//TIM4	J2	Left
int16_t Encoder_Get(TIM_TypeDef* TIMx)
{
	int16_t temp;
	temp=TIM_GetCounter(TIMx);
	TIM_SetCounter(TIMx,0);
	return temp;
}
