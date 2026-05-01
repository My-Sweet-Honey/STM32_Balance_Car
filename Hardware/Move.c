#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "PID.h"

extern int8_t Serial_RxPacket[3];
extern int16_t Exp_speed;
extern int8_t Set_turn;
uint8_t Stop_Flag=0;

void Motor_Ctrl(int16_t PWM_Left, int16_t PWM_Right)
{
	if(PWM_Left>0)
	{
		PWM_SetCompare_A(PWM_Left+33);
		GPIO_SetBits(GPIOA , GPIO_Pin_4);
		GPIO_ResetBits(GPIOA , GPIO_Pin_5);
	}
	else if(PWM_Left<0)
	{
		PWM_SetCompare_A(-PWM_Left+33);
		GPIO_SetBits(GPIOA , GPIO_Pin_5);
		GPIO_ResetBits(GPIOA , GPIO_Pin_4);
	}
	else
	{
		PWM_SetCompare_A(0);
		GPIO_ResetBits(GPIOA , GPIO_Pin_4 | GPIO_Pin_5);
	}
	
	if(PWM_Right>0)
	{
		PWM_SetCompare_B(PWM_Right+35);
		GPIO_SetBits(GPIOB , GPIO_Pin_0);
		GPIO_ResetBits(GPIOB , GPIO_Pin_1);
	}
	else if(PWM_Right<0)
	{
		PWM_SetCompare_B(-PWM_Right+35);
		GPIO_SetBits(GPIOB , GPIO_Pin_1);
		GPIO_ResetBits(GPIOB , GPIO_Pin_0);		
	}
	else
	{
		PWM_SetCompare_A(0);
		GPIO_ResetBits(GPIOB , GPIO_Pin_0 | GPIO_Pin_1);
	}

}

void Run_forward(void)
{
	Exp_speed=Serial_RxPacket[1]*2.5;
	Set_turn=0;
	Stop_Flag=0;
}

void Run_backward(void)
{
	Exp_speed=Serial_RxPacket[1]*2.5;
	Set_turn=0;
	Stop_Flag=0;
}

void Stop(void)
{
	Exp_speed=0;
	Set_turn=0;	
	Stop_Flag=1;
}

void Other_direction(void)
{
	Exp_speed=Serial_RxPacket[1]*1.7;
	Set_turn=Serial_RxPacket[0];
	if(Exp_speed<0)
		Set_turn=-Set_turn;
	Stop_Flag=0;
}
