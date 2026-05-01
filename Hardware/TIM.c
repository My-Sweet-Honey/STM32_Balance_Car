#include "stm32f10x.h"                  // Device header
#include "Encoder.h"  
#include "PID.h"  
#include "inv_mpu.h"
#include "Bluetooth.h"

extern int16_t Speed_Right;
extern int16_t Speed_Left;
extern float Pitch,Roll,Yaw;
extern int16_t GyroX,GyroZ;

void TIM1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 3600-1;		//ARR	1ms
	TIM_TimeBaseInitStructure.TIM_Prescaler = 20-1;	//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1,ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
	static uint8_t cnt=0;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)
	{
		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
		MPU6050_GetData(&GyroX, &GyroZ);
		GyroX+=40;
		GyroZ-=14;
		cnt++;
		if(cnt>=5)
		{
			cnt=0;
			Speed_Right=-Encoder_Get(TIM3);
			Speed_Left=-Encoder_Get(TIM4);		
		}
		BT_control();
		Car_Balance_Adjust();
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}
