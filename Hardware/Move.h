#ifndef __MOVE_H
#define __MOVE_H

void Motor_Ctrl(int16_t PWM_Left, int16_t PWM_Right);
void Run_forward(void);
void Run_backward(void);
void Stop(void);
void Other_direction(void);

#endif
