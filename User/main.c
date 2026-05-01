#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Bluetooth.h"
#include "PWM.h"
#include "Move.h"
#include "MPU6050.h"
#include "Encoder.h"
#include "inv_mpu.h"
#include "TIM.h"

float Pitch,Roll,Yaw;
int16_t GyroX,GyroZ;

extern int16_t Speed_Right;
extern int16_t Speed_Left;

extern int8_t Serial_RxPacket[3];

int main()
{
	PWM_Init();
	Serial_Init();
	OLED_Init();	
	delay_init();
	Encoder_Init();
	MPU6050_DMP_Init();
	Control_Init();
	TIM1_Init();			//必须放到最后初始化
	
	while(1)
	{
		OLED_ShowSignedNum(1,1,10*Roll,3);

		OLED_ShowSignedNum(1,6,GyroZ,4);
		
		OLED_ShowSignedNum(2,1,Speed_Left,5);
		OLED_ShowSignedNum(2,8,Speed_Right,5);
		
		OLED_ShowSignedNum(3,1,Serial_RxPacket[0],2);
		OLED_ShowSignedNum(3,5,Serial_RxPacket[1],2);
		OLED_ShowSignedNum(3,9,Serial_RxPacket[2],2);
		
		OLED_ShowHexNum(4,1,Serial_RxPacket[0],2);
		OLED_ShowHexNum(4,5,Serial_RxPacket[1],2);
		OLED_ShowHexNum(4,9,Serial_RxPacket[2],2);
	}
}
