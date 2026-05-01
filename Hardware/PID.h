#ifndef __PID_H
#define __PID_H

typedef struct{
	float Kp;
	float Kd;
	float PWM; 
}Vertical;

typedef struct{
	float Kp;
	float Ki;
	float PWM; 
}Velocity;

typedef struct{
	float Kp;
	float PWM; 
}Turn;

float Vertical_PID(float Angle, int16_t GyroX);
float Velocity_PID(int16_t Speed_Left, int16_t Speed_Right);
float Turn_PID(int8_t Set_turn);
void Car_Balance_Adjust(void);


#endif
