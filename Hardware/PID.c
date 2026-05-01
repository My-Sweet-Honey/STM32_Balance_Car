#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include "Move.h"

Vertical vertical={.Kp=10.5*0.6, .Kd=-0.018*0.6, .PWM=0};
Velocity velocity={.Kp=0.9, .Ki=0.9/100, .PWM=0};
Turn turn={.Kp=0.36, .PWM=0};

float Mid=-5;
int16_t Exp_speed=0;
int16_t PWM_Left=0;
int16_t PWM_Right=0;
int8_t Set_turn=0;

extern float Pitch,Roll,Yaw;
extern int16_t Speed_Right;
extern int16_t Speed_Left;
extern int16_t GyroX,GyroZ;
extern uint8_t Stop_Flag;

float Vertical_PID(float Angle, int16_t GyroX)
{
	float PWM_vertical = vertical.Kp * (Mid-Angle) + vertical.Kd * GyroX;
	return PWM_vertical;
}

float Velocity_PID(int16_t Speed_Left, int16_t Speed_Right)
{
	static int Encoder_Integral, Encoder, Encoder_last;
	Encoder = Speed_Left + Speed_Right - Exp_speed;
	Encoder_last = Encoder_last * 0.8 + Encoder*0.2;
	Encoder_Integral += Encoder_last;
	if(Stop_Flag==0)
		Encoder_Integral=0;
	float PWM_velocity = velocity.Kp * Encoder_last + velocity.Ki * Encoder_Integral;
	
	if(Encoder_Integral>10000)
	{
		Encoder_Integral=10000;
	}		
	if(Encoder_Integral<-10000)
	{
		Encoder_Integral=-10000;
	}
	if((Roll>=80)||(Roll<=-80))
	{
		Encoder_Integral=0;    
	}			

	return PWM_velocity;
}

float Turn_PID(int8_t Set_turn)
{
	float PWM_turn=0; 
	PWM_turn=turn.Kp * Set_turn; 
	return PWM_turn;
}

void Car_Balance_Adjust(void)
{
	vertical.PWM = Vertical_PID(Roll,GyroX);
	velocity.PWM = Velocity_PID(Speed_Left,Speed_Right);
	turn.PWM = Turn_PID(Set_turn);
	PWM_Left = (int16_t)(vertical.PWM + velocity.PWM + turn.PWM);
	PWM_Right = (int16_t)(vertical.PWM + velocity.PWM - turn.PWM);
	if(PWM_Left>=66)
	{
		PWM_Left=66;
	}
	else if(PWM_Left<=-66)
	{
		PWM_Left=-66;
	}
	
	if(PWM_Right>=64)
	{
		PWM_Right=64;
	}
	else if(PWM_Right<=-64)
	{
		PWM_Right=-64;
	}
	if((Roll>=80)||(Roll<=-80))
	{
		PWM_Left=0;
		PWM_Right=0;
	}
	Motor_Ctrl(PWM_Left,PWM_Right);
}
